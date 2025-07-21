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
	int memfd;
	if (-1 == (memfd = open("/dev/mem", O_RDWR))) {
		perror("open(/dev/mem)");
		return 1;
	};
	while (true) {
		char *full_command, *command_name, *save;
		full_command = readline("> ");
		command_name = strtok_r(full_command, separators, &save);
		if (command_name) {
			struct Command stub = (struct Command) { .name = command_name, .function = NULL};
			const struct Command *found = bsearch(&stub, commands, commands_count, sizeof commands[0], &command_searcher);
			if (found) {
				found->function(memfd, save);
			} else {
				puts("Wrong command");
			};
		};
		free(full_command);
	};
	close(memfd);
	return 0;
};
