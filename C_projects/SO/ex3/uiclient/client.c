#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include "../client/tecnicofs-client-api.h"
#define MAX_COMMAND_SIZE 100

#define MAX_OPEN_FILES 5


char *errors[11];

char fdsName[MAX_OPEN_FILES][MAX_COMMAND_SIZE];
int openedFiles = 0;
char line[MAX_COMMAND_SIZE];

int queryMenu() {
	int com;

	puts("");
	puts("\033[1;33m?Choose Command?\033[0m");
	puts("  1 - Create File");
	puts("  2 - Delete File");
	puts("  3 - Rename File");
	puts("  4 - Open File");
	puts("  5 - Close File");
	puts("  6 - Read from File");
	puts("  7 - Write in File");
	puts("  8 - Display current open files");
	puts("  0 - Exit");
	printf("Command(0-7)?: ");
	fgets(line, MAX_COMMAND_SIZE, stdin);
	sscanf(line, "%d", &com);
	puts("");

	return com < 0 || com > 8 ? -1 : com;
}

void queryCreate() {
	char name[MAX_COMMAND_SIZE];
	int owp, otp;
	printf("File name?: ");
	fgets(line, MAX_COMMAND_SIZE, stdin);
	sscanf(line, "%s\n", name);

	printf("Owner perms(0-3)?: ");
	fgets(line, MAX_COMMAND_SIZE, stdin);
	sscanf(line, "%d", &owp);

	printf("Other perms(0-3)?: ");
	fgets(line, MAX_COMMAND_SIZE, stdin);
	sscanf(line, "%d", &otp);

	int err = tfsCreate(name, owp, otp);

	if(err < 0)
		printf("\n\033[1;31mError:  %s\033[0m\n", errors[-err - 1]);
}


void queryDelete() {
	char name[MAX_COMMAND_SIZE];
	printf("File name?: ");
	fgets(line, MAX_COMMAND_SIZE, stdin);
	sscanf(line, "%s\n", name);

	int err = tfsDelete(name);

	if(err < 0)
		printf("\n\033[1;31mError:  %s\033[0m\n", errors[-err - 1]);
}

void queryRename() {
	char name[MAX_COMMAND_SIZE];
	char newname[MAX_COMMAND_SIZE];
	printf("File name?: ");
	fgets(line, MAX_COMMAND_SIZE, stdin);
	sscanf(line, "%s\n", name);
	printf("File name?: ");
	fgets(line, MAX_COMMAND_SIZE, stdin);
	sscanf(line, "%s\n", newname);

	int err = tfsRename(name, newname);

	if(err < 0)
		printf("\n\033[1;31mError:  %s\033[0m\n", errors[-err - 1]);

}

void queryOpen() {
	char name[MAX_COMMAND_SIZE];
	int mode;
	printf("File name?: ");
	fgets(line, MAX_COMMAND_SIZE, stdin);
	sscanf(line, "%s\n", name);
	printf("Open mode(0-3)?: ");
	fgets(line, MAX_COMMAND_SIZE, stdin);
	sscanf(line, "%d", &mode);

	int status = tfsOpen(name, mode);

	if(status < 0) {
		printf("\n\033[1;31mError:  %s\033[0m\n", errors[-status - 1]);
		return;
	}
		
	strcpy(fdsName[status], name);
}

void listFds() {
	puts("\033[0;33m|| Current fds ||\033[0m");
	for(int x = 0; x < MAX_OPEN_FILES; x++) 
		if(fdsName[x][0] != -1)
			printf("File:%s --> %d\n", fdsName[x], x);
}

int getFds() {
	listFds();

	int choosefds;
	printf("Choose fds?: ");
	fgets(line, MAX_COMMAND_SIZE, stdin);
	sscanf(line, "%d", &choosefds);

	return choosefds;
}

void queryClose() {	
	int closefd = getFds();
	
	int err = tfsClose(closefd);

	if(err < 0) {
		printf("\n\033[1;31mError:  %s\033[0m\n", errors[-err - 1]);
		return;
	}

	fdsName[closefd][0] = -1;

}

void queryRead() {
	int readfd = getFds();

	char buff[MAX_COMMAND_SIZE];
	int len;

	printf("Read how many characters(Includes \\0)?: ");
	fgets(line, MAX_COMMAND_SIZE, stdin);
	sscanf(line, "%d", &len);

	int status = tfsRead(readfd, buff, len);
	if(status < 0) {
		printf("\n\033[1;31mError:  %s\033[0m\n", errors[-status - 1]);
		return;
	}

	printf("Read: %s, %d chars long\n", buff, status);
}

void queryWrite() {
	int writefd = getFds();

	char buff[MAX_COMMAND_SIZE];
	int len;

	printf("Write what content?: ");
	fgets(line, MAX_COMMAND_SIZE, stdin);
	sscanf(line, "%[^\n]\n", buff);
	puts(buff);

	printf("Write how many characters(Includes \\0)?: ");
	fgets(line, MAX_COMMAND_SIZE, stdin);
	sscanf(line, "%d", &len);

	int err = tfsWrite(writefd, buff, len);

	if(err < 0)
		printf("\n\033[1;31mError:  %s\033[0m\n", errors[-err - 1]);
}

void endProgramSIGINT(int arg) {
	puts("");
	tfsUnmount();
	exit(0);
}


int main(int argc, char *argv[]) {
	signal(SIGINT, endProgramSIGINT);
	errors[0] = "TECNICOFS_ERROR_OPEN_SESSION";
	errors[1] = "TECNICOFS_ERROR_NO_OPEN_SESSION";
	errors[2] = "TECNICOFS_ERROR_CONNECTION_ERROR";
	errors[3] = "TECNICOFS_ERROR_FILE_ALREADY_EXISTS";
	errors[4] = "TECNICOFS_ERROR_FILE_NOT_FOUND";
	errors[5] = "TECNICOFS_ERROR_PERMISSION_DENIED";
	errors[6] = "TECNICOFS_ERROR_MAXED_OPEN_FILES";
	errors[7] = "TECNICOFS_ERROR_FILE_NOT_OPEN";
	errors[8] = "TECNICOFS_ERROR_FILE_IS_OPEN";
	errors[9] = "TECNICOFS_ERROR_INVALID_MODE";
	errors[10] = "TECNICOFS_ERROR_OTHER";
	// Para adicionar erros aumentem o tamanho da var errors e coloquem aqui por ordem
	// indice == -err -1 aka para erro -3 o indice fica 2

	
	printf("Mounting to socket: \033[0;33;40m%s\033[0m\n", argv[1]);

	int com = tfsMount(argv[1]);
	if(com < 0) {
		puts("\033[1;31mError\033[0m mounting to socket");
		exit(1);
	}
	puts("Socket mounting \033[1;32msuccessful\033[0m");

	for(int x = 0; x < MAX_OPEN_FILES; x++)
		fdsName[x][0] = -1;

	while((com = queryMenu())) {
		switch(com) {
			case 1:
				queryCreate();
				break;

			case 2:
				queryDelete();
				break;

			case 3:
				queryRename();
				break;

			case 4:
				queryOpen();
				break;

			case 5:
				queryClose();
				break;

			case 6:
				queryRead();
				break;

			case 7:
				queryWrite();
				break;

			case 8:
				listFds();
				break;
		}
	}

	tfsUnmount();
}