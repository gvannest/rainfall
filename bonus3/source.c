#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
	char buf[132]; // at esp+0x18
	FILE *fd = fopen("/home/user/end/.pass", "r");
	bzero(buf, 132); // note 132 = 4*0x21. ecx is decremented for each 4 bytes copied
	if (argc == 2 && fd != NULL)
	{
		fread(buf, 1, 66, fd);
		buf[66] = 0;
		buf[atoi(argv[1])] = 0;
		fread((buf + 66), 1, 65, fd);
		fclose(fd);
		if (strcmp(buf, argv[1]) == 0) { // c'est ici que je dois rentrer :)
			execl("/bin/sh", "sh", NULL);
		} else {
			puts(buf + 66); //normal que je n'ai rien : le deuxieme read ne lit rien car l'indicateur de position est avance du nombre d'octets lus precedemment!!
		}
		return 0;
	}
	return -1;
}
