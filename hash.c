#include <stdio.h>
#include <string.h>
#include <stdlib.h>


size_t hash(char *name, char *id) {
	char *res = NULL;
//	int sum = 0;
	size_t result = 533436788;
	size_t size = strlen(name) + strlen(id) + 1;

    res = (char *)malloc(size);
	memset(res, 0, size);
	strcpy(res, name);
	strcat(res, id);
/*
	for(int i = 0; i < strlen(name); ++i) {
		sum ^= name[i];
	}
*/
	
	printf("res str = %s\n", res);

	for(int i = 0; i < strlen(res); ++i) {
		result = (result << 2) * res[i] ^ (result >> 1);
	}

	free(res);

	return result;
}


int main(int argc, char **argv) {
	printf("res = %zu\n", hash(argv[1], argv[2]));

	return 0;
}
