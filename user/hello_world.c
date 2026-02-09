#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void demo_file_read(char *filename) {
	int fd;
	char buf[128];
	int n;

	fd = open(filename,0);
	if(fd<0){
		printf("hello_world: cannot open %s/n", filename);
		return;
	}
	while((n=read(fd, buf, sizeof(buf))) > 0) {
		write(1, buf, n);
	}
	close (fd);
}
int main(int argc, char *argv[]) {
	printf("Hello world from xv6!\n");
	
	if (argc < 2){
		printf("Usage: hello_world <filename>\n");
		exit(0);
	}

	demo_file_read(argv[1]);
	exit(0);
}
