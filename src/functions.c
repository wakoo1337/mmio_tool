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
void cmd_exit(int portfd, char **save) {
	close(portfd);
	exit(0);
};
void cmd_help(int portfd, char **save) {
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
bool read_bytes(int portfd, off_t offset, void *out, unsigned int count) {
	if (-1 == lseek(portfd, offset, SEEK_SET)) {
		perror("lseek");
		return true;
	};
	if (count != read(portfd, out, count)) {
		perror("read");
		return true;
	};
	return false;
};
bool write_bytes(int portfd, off_t offset, void *in, unsigned int count) {
	if (-1 == lseek(portfd, offset, SEEK_SET)) {
		perror("lseek");
		return true;
	};
	if (count != write(portfd, in, count)) {
		perror("write");
		return true;
	};
	return false;
};
bool query_address(char **save, off_t *address) {
	char *address_txt;
	address_txt = strtok_r(NULL, separators, save);
	if (NULL == address_txt) {
		return true;
	};
	char *endptr;
	*address = (off_t) strtoull(address_txt, &endptr, 0);
	return endptr == address_txt;
};
bool query_argument(char **save, uint32_t *argument) {
	char *argument_txt;
	argument_txt = strtok_r(NULL, separators, save);
	if (NULL == argument_txt) {
		return true;
	};
	char *endptr;
	*argument = (uint32_t) strtoull(argument_txt, &endptr, 0);
	return endptr == argument_txt;
};
void cmd_iorb(int portfd, char **save) {
	off_t offset;
	if (query_address(save, &offset)) {
		puts("Wrong or absent address specification");
		return;
	};
	unsigned char c;
	if (!read_bytes(portfd, offset, &c, sizeof c)) {
		printf("readed byte: %02hhx\n", c);
	};
};
void cmd_iorw(int portfd, char **save) {
	off_t offset;
	if (query_address(save, &offset)) {
		puts("Wrong or absent address specification");
		return;
	};
	uint16_t u;
	if (!read_bytes(portfd, offset, &u, sizeof u)) {
		printf("readed word: %04hx\n", u);
	};
};
void cmd_iord(int portfd, char **save) {
	off_t offset;
	if (query_address(save, &offset)) {
		puts("Wrong or absent address specification");
		return;
	};
	uint32_t u;
	if (!read_bytes(portfd, offset, &u, sizeof u)) {
		printf("readed double word: %08x\n", u);
	};
};
void cmd_iowb(int portfd, char **save) {
	off_t offset;
	if (query_address(save, &offset)) {
		puts("Wrong or absent address specification");
		return;
	};
	uint32_t u32;
	if (query_argument(save, &u32)) {
		puts("Wrong or absent argument");
		return;
	};
	uint8_t u8;
	u8 = u32;
	if (!write_bytes(portfd, offset, &u8, sizeof u8)) {
		puts("wrote byte successfully");
	};
};
void cmd_ioww(int portfd, char **save) {
	off_t offset;
	if (query_address(save, &offset)) {
		puts("Wrong or absent address specification");
		return;
	};
	uint32_t u32;
	if (query_argument(save, &u32)) {
		puts("Wrong or absent argument");
		return;
	};
	uint16_t u16;
	u16 = u32;
	if (!write_bytes(portfd, offset, &u16, sizeof u16)) {
		puts("wrote byte successfully");
	};
};
void cmd_iowd(int portfd, char **save) {
	off_t offset;
	if (query_address(save, &offset)) {
		puts("Wrong or absent address specification");
		return;
	};
	uint32_t u32;
	if (query_argument(save, &u32)) {
		puts("Wrong or absent argument");
		return;
	};
	if (!write_bytes(portfd, offset, &u32, sizeof u32)) {
		puts("wrote byte successfully");
	};
};
