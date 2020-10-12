#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void p(void){
	fflush(stdout);
	char buf[76];
	gets(buf);
	char *p = (buf + 84);
	uintptr_t ptr = (uintptr_t)p;
	if ((ptr & 0xb0000000) == 0xb0000000) { // it basically checks whether the next instruction in the main funciton starts with 0xb -> it would mean it is a stack based instruction. protection
		printf("%p \n", p);
		exit(1);
	} else {
		strdup(buf);
	}
}

int main(void) {
	p();
	return 0;
}
