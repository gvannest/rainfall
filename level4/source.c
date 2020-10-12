#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int *ptr; // value set to 0x8049810 cf line <+54> in n() disas

void p(char *buf){
	printf(buf);
}

void n(void){
	char buf[520];
	fgets(buf, 512, stdin);
	p(buf);

	if (*ptr == 0x1025544) {
		system("/bin/cat /home/user/level5/.pass");
	}
	return;
}

int main(void){
	n();
	return 0;
}
