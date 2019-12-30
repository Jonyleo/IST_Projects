#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "tecnicofs-api-constants.h"

#define MAX_COMMAND_SIZE 100
#define CREATE_COMMAND 'c'
#define DELETE_COMMAND 'd'
#define RENAME_COMMAND 'r'
#define OPEN_COMMAND 'o'
#define CLOSE_COMMAND 'x'
#define READ_COMMAND 'l'
#define WRITE_COMMAND 'w'

int sockfd = -1;

int sendAndReceive(char *payload, int writeSize) {
	int err;

	if(write(sockfd, payload, writeSize) != writeSize)
		return TECNICOFS_ERROR_OTHER;

	if(read(sockfd, &err, sizeof(int)) != sizeof(int))
		return TECNICOFS_ERROR_OTHER;
	
	return err;
}


int tfsCreate(char *filename, permission ownerPermissions, permission othersPermissions) {
	if(sockfd == -1)
		return TECNICOFS_ERROR_NO_OPEN_SESSION;

	if(ownerPermissions > 3 || ownerPermissions < 0 || othersPermissions > 3 || othersPermissions < 0)
		return TECNICOFS_ERROR_OTHER;

	if(filename == NULL)
		return TECNICOFS_ERROR_OTHER;

	char command[MAX_COMMAND_SIZE];
	int n = sprintf(command, "%c %s %d%d", CREATE_COMMAND, filename, ownerPermissions, othersPermissions);
	
	return sendAndReceive(command, n);
}

int tfsDelete(char *filename) {
	if(sockfd == -1)
		return TECNICOFS_ERROR_NO_OPEN_SESSION;

	if(filename == NULL)
		return TECNICOFS_ERROR_OTHER;

	char command[MAX_COMMAND_SIZE];
	int n = sprintf(command,"%c %s", DELETE_COMMAND, filename);

	
	return sendAndReceive(command, n);
}

int tfsRename(char *filenameOld, char *filenameNew) {
	if(sockfd == -1)
		return TECNICOFS_ERROR_NO_OPEN_SESSION;

	if(filenameOld == NULL)
		return TECNICOFS_ERROR_OTHER;

	if(filenameNew == NULL)
		return TECNICOFS_ERROR_OTHER;

	char command[MAX_COMMAND_SIZE];
	int n = sprintf(command, "%c %s %s", RENAME_COMMAND, filenameOld, filenameNew);

	
	return sendAndReceive(command, n);
}

int tfsOpen(char *filename, permission mode) {
	if(sockfd == -1)
		return TECNICOFS_ERROR_NO_OPEN_SESSION;

	if(filename == NULL)
		return TECNICOFS_ERROR_OTHER;

	if(mode < 0 || mode > 3)
		return TECNICOFS_ERROR_OTHER;

	char command[MAX_COMMAND_SIZE];
	int n = sprintf(command, "%c %s %d", OPEN_COMMAND, filename, mode);

	return sendAndReceive(command, n);
}

int tfsClose(int fd) {
	if(sockfd == -1)
		return TECNICOFS_ERROR_NO_OPEN_SESSION;

	if(fd < 0)
		return TECNICOFS_ERROR_OTHER;

	char command[MAX_COMMAND_SIZE];
	int n = sprintf(command, "%c %d", CLOSE_COMMAND, fd);

	return sendAndReceive(command, n);
}

int tfsRead(int fd, char *buffer, int len) {
	if(sockfd == -1)
		return TECNICOFS_ERROR_NO_OPEN_SESSION;

	if(fd < 0)
		return TECNICOFS_ERROR_OTHER;

	if(len < 0)
		return TECNICOFS_ERROR_OTHER;

	char command[MAX_COMMAND_SIZE];
	int n = sprintf(command, "%c %d %d", READ_COMMAND, fd, len);

	n = sendAndReceive(command, n);

	if(n <= 0)
		return n;

	if(read(sockfd, buffer, n) != n)
		return TECNICOFS_ERROR_OTHER;

	buffer[n] = '\0';

	return n;

}
int tfsWrite(int fd, char *buffer, int len) {
	if(sockfd == -1)
		return TECNICOFS_ERROR_NO_OPEN_SESSION;

	if(buffer == NULL)
		return TECNICOFS_ERROR_OTHER;

	if(len < 0)
		return TECNICOFS_ERROR_OTHER;

	if(len == 0)
		return 0;

	char command[MAX_COMMAND_SIZE];
	int n = sprintf(command, "%c %d %.*s", WRITE_COMMAND, fd, len, buffer);

	return sendAndReceive(command, n);
}


int tfsMount(char *address) {
	if(sockfd != -1)
		return TECNICOFS_ERROR_OPEN_SESSION;

	if(address == NULL)
		return TECNICOFS_ERROR_OTHER;

	int servlen;
	struct sockaddr_un serv_addr;

	if((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
		return TECNICOFS_ERROR_CONNECTION_ERROR;

	serv_addr.sun_family = AF_UNIX;
	strcpy(serv_addr.sun_path, address);
	servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);

	if(connect(sockfd, (struct sockaddr *) &serv_addr, servlen) < 0)
		return TECNICOFS_ERROR_CONNECTION_ERROR;

	return 0;
}


int tfsUnmount() {
	int err = sendAndReceive("q", 1);
	close(sockfd);
	return err;
}
