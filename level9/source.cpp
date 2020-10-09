#include <string.h>
#include <stdlib.h>

class N {
	public:
		unsigned int x;
		N(unsigned int y) : x(y){}
		N operator+(N& m) { return *(new N(this->x + m.x));}
		N operator-(N& m) { return *(new N(this->x - m.x));}
		void *setAnnotation(char *str) { return memcpy(this + 0x4, str, strlen(str)); }
};


int main(int argc, char **argv){
	if (argc < 2){
		exit(1);
	}
	N *n = new N(5);
	N *m = new N(6);
	n->setAnnotation(argv[1]);
	*m + *n;
	return 0;
}
