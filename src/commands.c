#include "Command.h"
#include "commands.h"
#include "functions.h"
const int commands_count = 8;
const struct Command commands[] = {
	 (struct Command) { .name = "exit", .function = &cmd_exit },
	 (struct Command) { .name = "help", .function = &cmd_help },
	 (struct Command) { .name = "iorb", .function = &cmd_iorb },
	 (struct Command) { .name = "iord", .function = &cmd_iord },
	 (struct Command) { .name = "iorw", .function = &cmd_iorw },
	 (struct Command) { .name = "iowb", .function = &cmd_iowb },
	 (struct Command) { .name = "iowd", .function = &cmd_iowd },
	 (struct Command) { .name = "ioww", .function = &cmd_ioww },
};
