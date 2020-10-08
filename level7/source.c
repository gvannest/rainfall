#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<time.h>


char str[100];

typedef struct	stru
{
	int		n;
	void	*ptr;
}				t_stru;

void m(void) {
	printf("%s - %d\n", str, time(0));
}

int main(int argc, char **argv) {
	t_stru *s1 = malloc(sizeof(t_stru));
	s1->n = 0x1;
	s1->ptr = malloc(0x8);

	t_stru *s2 = malloc(sizeof(t_stru));
	s2->n = 0x2;
	s2->ptr = malloc(0x8);

	strcpy(s1->ptr, argv[1]);
	strcpy(s2->ptr, argv[2]); 

	FILE *fd = fopen("/home/user/level8/.pass", "r");
	fgets(str, 68, fd);
	puts("~~");
}
