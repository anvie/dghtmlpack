
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "htmlpacker.h"


int main(int argc, char** argv){
	
	FILE* fd;
	int rv;
	struct stat fst;
	char* src_buff;
	char* dst_buff;
	size_t brw;
	
	fd = fopen("robin.html", "r");
	if (fd == 0){
		printf("cannot open file");
		return 1;
	}
	
	if(fstat(fileno(fd), &fst) < 0){
		fclose(fd);
		return 2;
	}
	
	src_buff = calloc(fst.st_size, sizeof(char));
	if (src_buff == 0) {
		fclose(fd);
		return 3;
	}
	
	brw = fread(src_buff, fst.st_size, sizeof(char), fd);
	
	dst_buff = calloc(fst.st_size + 8, sizeof(char));
	if (dst_buff == 0) {
		fclose(fd);
		free(src_buff);
		return 4;
	}
	
	rv = do_html_pack(src_buff, fst.st_size, dst_buff, fst.st_size);
	
	fclose(fd);
	
	printf("Result:\n%s\n", dst_buff);
	
	return 0;
}

