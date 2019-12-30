#include "hash.h"

/* Simple hash function for strings.
 * Receives a string and resturns its hash value
 * which is a number between 0 and n-1
 * In case the string is null, returns -1 */
int hash(char* name, int n) {
	if (!name) 
		return -1;
	return (int) name[0] % n;
}


