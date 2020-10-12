
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int language = 0;

int greetuser(char *str){
	char buf[72];
	if (language == 1)
		strncpy(buf, "HyvÃÃ pÃivÃÃ", 19); // 19 because of unicode : check x/s 0x8048717
	if (language == 2)
		strncpy(buf, "Goedemiddag! ", 14);
	else
		strncpy(buf, "Hello ", 7);
	strcat(buf, str);
	puts(buf);
	return 0;
}

int main (int argc, char **argv) {
	char buf[72]; // at $esp + 0x50 => Line<+31>
	char *l;
	if (argc == 3){
		bzero(buf, 72); // Line<+35> to <+49>. 0x13 = 19 => ecx is decremented for each 4 bytes copied! 19 * 4 = 76, but the last one is 0 and get out
		strncpy(buf, argv[1], 40); // 0x28 = 40   --   Line<+78>
		strncpy(buf + 40, argv[2], 32); // 0x20 = 32    --   Line <+113>
		l = getenv("LANG");
		if (l) {
			if (memcmp(l, "fi", 0x2) == 0)
				language = 0x1;
			else if (memcmp(l, "nl", 0x2) == 0)
				language = 0x2;
		}
		return (greetuser(buf));
	}
	return 1;
}
