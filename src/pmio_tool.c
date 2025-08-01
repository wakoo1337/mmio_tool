#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "Command.h"
#include "commands.h"
#include "separators.h"

int command_searcher(const void *a, const void *b) {
	return strcmp(((struct Command *) a)->name, ((struct Command *) b)->name);
};
int main(int argc, char **argv) {
	if (getuid()) {
		puts("You must be root to use mmio_tool");
		return 1;
	};
	int portfd;
	if (-1 == (portfd = open("/dev/port", O_RDWR))) {
		perror("open(/dev/port)");
		return 1;
	};
	while (true) {
		char *full_command, *command_name, *save;
		fputs("> ", stdout);
		fflush(stdout);
		full_command = NULL;
		size_t sz;
		if (-1 == getline(&full_command, &sz, stdin)) {
			if (feof(stdin)) {
				free(full_command);
				close(portfd);
				return 0;
			} else if (ferror(stdin)) {
				perror("getline");
				free(full_command);
				close(portfd);
				return 1;
			};
		};
		command_name = strtok_r(full_command, separators, &save);
		if (command_name) {
			struct Command stub = (struct Command) { .name = command_name, .function = NULL};
			const struct Command *found = bsearch(&stub, commands, commands_count, sizeof commands[0], &command_searcher);
			if (found) {
				found->function(portfd, &save);
			} else {
				puts("Wrong command");
			};
		};
		free(full_command);
	};
	close(portfd);
	return 0;
};
