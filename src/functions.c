#include <stdint.h>
#include <stdbool.h>
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
bool read_bytes(int memfd, char *save, unsigned char *out, unsigned int count) {
	char *address_txt;
	address_txt = strtok_r(NULL, separators, &save);
	if (NULL == address_txt) {
		puts("Specify address");
		return true;
	};
	char *endptr;
	intptr_t address = strtoull(address_txt, &endptr, 0); // unsigned long long is 64-bit, so it's possible to keep pointer there
	if (endptr == address_txt) {
		puts("Invalid address specification");
		return true;
	};
	intptr_t page = getpagesize();
	intptr_t aligned = address & ~(page-1);
	size_t length = (address & (page-1)) + count;
	void *mapping;
	mapping = mmap(NULL, length, PROT_READ, MAP_SHARED, memfd, aligned);
	if (MAP_FAILED == mapping) {
		perror("mmap");
		return true;
	};
	memcpy(out, mapping + (address & (page-1)), count);
	munmap(mapping, length);
	return false;
};
void cmd_iorb(int memfd, char *save) {
	unsigned char c;
	if (!read_bytes(memfd, save, &c, 1)) {
		printf("readed byte: %hhx\n:", c);
	};
};
void cmd_iorw(int memfd, char *save) {
	union {
		uint16_t u;
		unsigned char c[2];
	} u;
	if (!read_bytes(memfd, save, u.c, 2)) {
		printf("readed word: %hx\n", u.u);
	};
};
void cmd_iord(int memfd, char *save) {
	union {
		uint32_t u;
		unsigned char c[4];
	} u;
	if (!read_bytes(memfd, save, u.c, 4)) {
		printf("readed double word: %x\n", u.u);
	};
};
void cmd_iowb(int memfd, char *save) {
};
void cmd_iowd(int memfd, char *save) {
};
void cmd_ioww(int memfd, char *save) {
};
