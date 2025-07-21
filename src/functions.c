#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include "separators.h"

#include "functions.h"
void cmd_exit(int memfd, char *save) {
	exit(0);
};
void cmd_help(int memfd, char *save) {
	puts("Available commands: \n\
  exit              — Exit program\n\
  help              — View this help\n\
  iorb address      — Read byte (8 bits)\n\
  iorw address      — Read word (16 bits) \n\
  iord address      — Read double word (32 bits)\n\
  iowb address data — Write byte (8 bits)\n\
  ioww address data — Write word (16 bits)\n\
  iowd address data — Write double word (32 bits)\n");
};
void cmd_iorb(int memfd, char *save) {
	char *address_txt;
	address_txt = strtok_r(NULL, separators, &save);
	if (NULL == address_txt) {
		puts("Specify address");
		return;
	};
	char *endptr;
	intptr_t address = strtoull(address_txt, &endptr, 0); // unsigned long long is 64-bit, so it's possible to keep pointer there
	if (endptr == address_txt) {
		puts("Invalid address specification");
		return;
	};
	intptr_t page = getpagesize();
	intptr_t aligned = address & ~(page-1);
	size_t length = (address & (page-1)) + 1;
	void *mapping;
	mapping = mmap(NULL, length, PROT_READ, MAP_SHARED, memfd, aligned);
	if (MAP_FAILED == mapping) {
		perror("mmap");
		return;
	};
	unsigned char value;
	value = ((unsigned char *) mapping)[address & (page-1)];
	munmap(mapping, length);
};
void cmd_iord(int memfd, char *save) {
};
void cmd_iorw(int memfd, char *save) {
};
void cmd_iowb(int memfd, char *save) {
};
void cmd_iowd(int memfd, char *save) {
};
void cmd_ioww(int memfd, char *save) {
};
