#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	char str[40];
	int n = atoi(argv[1]);
	if (n <= 0x9) {
		memcpy(str, argv[2], n * 0x4); // Note : str is located 40 bytes above the location where n is stored
		if (n == 0x574f4c46) // need to erase n in memory (right after str in stack) with this value
			execl("sh", "/bin/sh", NULL);
	} else {
		return 1;
	}
	return 0;
}
