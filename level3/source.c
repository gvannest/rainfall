#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int *ptr; //stored at address 0x804988c

void v(void){
	char buf[520];
	fgets(buf, 512, stdin);
	printf(buf);
	
	if (*ptr == 64) {
		fwrite("Wait what?!\n", 1, 12, stdout);
		system("/bin/sh");
	}
	return;
}

int main(void){
	v();
	return 0;
}
