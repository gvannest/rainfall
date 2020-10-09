#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void n(void) {
	system("/bin/cat /home/user/level7/.pass");
}

void m(void){
	puts("Nope");
}

int main(int argc, char **argv) {
	void *ptr;
	void *ptr2;
	ptr = malloc(64);
	ptr2 = malloc(4);
	ptr2 = &m;
	strcpy(ptr, argv[1]);
	(*ptr2)();
	return 0;
}
