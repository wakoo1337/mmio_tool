#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
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
  iorw address      — Read word (16 bits)\n\
  iord address      — Read double word (32 bits)\n\
  iowb address data — Write byte (8 bits)\n\
  ioww address data — Write word (16 bits)\n\
  iowd address data — Write double word (32 bits)");
};
void *map_view(int memfd, intptr_t address, unsigned int count, int protection, unsigned int *offset_out, unsigned int *length_out) {
	intptr_t page = getpagesize();
	intptr_t aligned = address & ~(page-1);
	size_t length = (address & (page-1)) + count;
	void *mapping;
	mapping = mmap(NULL, length, protection, MAP_SHARED, memfd, aligned);
	if (MAP_FAILED == mapping) {
		perror("mmap");
		return NULL;
	};
	*offset_out = address & (page-1);
	*length_out = length;
	return mapping;
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
	unsigned int offset, length;
	unsigned char *map;
	map = map_view(memfd, address, count, PROT_READ, &offset, &length);
	if (map) {
		memcpy(out, map + offset, count);
		munmap(map, length);
		return false;
	};
	return true;
};
bool write_bytes(int memfd, char *save, unsigned int count) {
	assert((count == 1) || (count == 2) || (count == 4));
	char *address_txt;
	address_txt = strtok_r(NULL, separators, &save);
	if (NULL == address_txt) {
		puts("Specify address");
		return true;
	};
	char *endptr;
	intptr_t address = strtoull(address_txt, &endptr, 0);
	if (endptr == address_txt) {
		puts("Invalid address specification");
		return true;
	};
	char *value_txt;
	value_txt = strtok_r(NULL, separators, &save);
	if (NULL == value_txt) {
		puts("Specify value");
		return true;
	};
	uint32_t value = strtoul(value_txt, &endptr, 0);
	if (endptr == value_txt) {
		puts("Invalid value specification");
		return true;
	};
	unsigned int offset, length;
	unsigned char *map;
	map = map_view(memfd, address, count, PROT_WRITE, &offset, &length);
	if (map) {
		unsigned char c;
		uint16_t s;
		uint32_t w;
		c = value;
		s = value;
		w = value;
		if (count == 1) memcpy(map + offset, &c, count);
		else if (count == 2) memcpy(map + offset, &s, count);
		else if (count == 4) memcpy(map + offset, &w, count);
		munmap(map, length);
		return false;
	};
	return true;
};
void cmd_iorb(int memfd, char *save) {
	unsigned char c;
	if (!read_bytes(memfd, save, &c, 1)) {
		printf("readed byte: %02hhx\n", c);
	};
};
void cmd_iorw(int memfd, char *save) {
	union {
		uint16_t u;
		unsigned char c[2];
	} u;
	if (!read_bytes(memfd, save, u.c, 2)) {
		printf("readed word: %04hx\n", u.u);
	};
};
void cmd_iord(int memfd, char *save) {
	union {
		uint32_t u;
		unsigned char c[4];
	} u;
	if (!read_bytes(memfd, save, u.c, 4)) {
		printf("readed double word: %08x\n", u.u);
	};
};
void cmd_iowb(int memfd, char *save) {
	if (!write_bytes(memfd, save, 1)) {
		puts("wrote byte successfully");
	};
};
void cmd_ioww(int memfd, char *save) {
	if (!write_bytes(memfd, save, 2)) {
		puts("wrote word successfully");
	};
};
void cmd_iowd(int memfd, char *save) {
	if (!write_bytes(memfd, save, 4)) {
		puts("wrote double word successfully");
	};
};
