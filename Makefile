CFLAGS := -O0 -g -Wall -std=gnu11
.PHONY: all clean
all: bin/pmio_tool
bin/pmio_tool: bin/pmio_tool.o bin/commands.o bin/functions.o bin/separators.o
	$(CC) $(CFLAGS) -o bin/pmio_tool bin/pmio_tool.o bin/commands.o bin/functions.o bin/separators.o
bin/pmio_tool.o: src/pmio_tool.c src/Command.h src/commands.h src/separators.h
	$(CC) $(CFLAGS) -c -o bin/pmio_tool.o src/pmio_tool.c
bin/commands.o: src/commands.c src/Command.h src/commands.h src/functions.h
	$(CC) $(CFLAGS) -c -o bin/commands.o src/commands.c
src/commands.c: make_commands.py
	python3 make_commands.py
bin/functions.o: src/functions.c src/functions.h src/separators.h
	$(CC) $(CFLAGS) -c -o bin/functions.o src/functions.c
bin/separators.o: src/separators.c src/separators.h
	$(CC) $(CFLAGS) -c -o bin/separators.o src/separators.c
clean:
	rm -vf src/commands.c bin/*
