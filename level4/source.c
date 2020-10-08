#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void p(char *buf){
	printf(buf);
}

void v(void){
	char buf[520];
	fgets(buf, 512, stdin);
	p(buf);

	void *ptr = (void*)0x8049810;
	if (*(int*)ptr == 0x1025544) {
		system("/bin/cat /home/user/level5/.pass");
	}
	return;
}

int main(void){
	v();
	return 0;
}
