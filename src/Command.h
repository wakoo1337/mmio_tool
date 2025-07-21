struct Command {
	const char *name;
	void (*function)(int, char *);
};
