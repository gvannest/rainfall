#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char ** argv) {
	char *ptr;
	int n = atoi(argv[1]);
	if (n == 423) {
		ptr = strdup("/bin/sh");
		gid_t gid = getegid();
		uid_t uid = geteuid();
		setresgid(gid, gid, gid);
		setresuid(uid, uid, uid);
		execv(ptr, &ptr);
	} else {
		fwrite("No !\n", 1, 5, stderr);
	}
	return 0;
}

