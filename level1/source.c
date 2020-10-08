#include <stdio.h>
#include <stdlib.h>

void run(void) {
	fwrite("Good... Wait what?\n", 1, 13, stdout);
	system("/bin/sh");
	return;
}

int main(void) {
	char buffer[64];
	gets(buffer);
	return 0;
}
