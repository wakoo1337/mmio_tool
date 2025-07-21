#!/usr/bin/python3

import string

class Command:
    def __init__(self, name, function):
        self.name = name
        self.function = function

commands = sorted([
    Command("exit", "cmd_exit"),
    Command("help", "cmd_help"),
    Command("iorb", "cmd_iorb"),
    Command("iorw", "cmd_iorw"),
    Command("iord", "cmd_iord"),
    Command("iowb", "cmd_iowb"),
    Command("ioww", "cmd_ioww"),
    Command("iowd", "cmd_iowd")
    ], key=lambda cmd: cmd.name.encode("ascii"))

with open("src/commands.c", mode="w", encoding="utf-8", newline="\n") as fd:
    fd.write(f"""#include \"Command.h\"
#include \"commands.h\"
#include \"functions.h\"
const int commands_count = {len(commands)};
const struct Command commands[] = {{\n""")
    cmd_template = string.Template("\t (struct Command) { .name = \"$name\", .function = &$function },\n")
    for command in commands:
        fd.write(cmd_template.substitute(name=command.name, function=command.function))
    fd.write("};\n")
