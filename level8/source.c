#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *auth;
char *serv;

int main(void){
	char buf[128];
	while (1) {
		printf("%p %p\n", auth, serv);
		char *s = fgets(buf, 0x80, stdin);
		if (strncmp(s, "auth ", 5) == 0) {
			auth = (char*)malloc(0x4);
			*(int*)auth = 0x0;
			if (strlen(s + 0x5) < 30)
				strcpy(auth, (s + 0x5));
		}
		if (strncmp(s, "reset", 5) == 0)
			free(auth);
		if (strncmp(s, "service", 6) == 0)
			serv = strdup(s + 7);
		if (strncmp(s, "login", 5) == 0){
			if (*(auth + 0x20) != 0)
				system("/bin/sh");
		}
	}
	return 0;
}
