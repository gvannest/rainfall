#include <string.h>
#include <stdio.h>
#include <unistd.h>

void p(char *dst, char *str){
	puts(str);
	char buf[4096]; //<+28>
	read(0x0, buf, 4095);
	*strchr(buf, '\n') = '\0'; //0xa is Line feed
	strncpy(dst, buf, 20); // NOTe : if no NULL byte in buf2 then the string in buf won't be null terminated! => exploit
}

void pp(char *mainbuf){
	char str1[20];
	char str2[20];// <+16> ces 2 buffers proviennent d'un buf de 48o sur la stack
	p(str1, " - ");
	p(str2, " - "); //0x14 = 0x30 - 0x1c
	strcpy(mainbuf, str1); //as first arg to function, mainbuf is at $ebp+0x8) => if first part longer than 20, then 2nd part is already copied here!
	int len = strlen(mainbuf); //<+69> to <+97>
	mainbuf[len] = ' '; //<+100> to <+106>
	mainbuf[len + 1] = '\0'; //the space is actually 2bytes long and is a string: " " which contains a \0. the program copies both bytes at len
	strcat(mainbuf, str2); // append string 2nd argument to dirst argument. first argument should be long enough after \0. 2nd arg (appended str) will erase \0 then sets its own
}


int main(void){
	char sbuf[50];
	pp(sbuf);
	puts(sbuf);
}
