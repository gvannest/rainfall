#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void v(void){
	char buf[520];
	fgets(buf, 512, stdin);
	printf(buf);
	
	void *ptr = (void*)0x804988c;
	if (*(int*)ptr == 64) {
		fwrite("Wait what?!\n", 1, 12, stdout);
		system("/bin/sh");
	}
	return;
}

int main(void){
	v();
	return 0;
}
