#ifndef COMMANDS_H
#define COMMANDS_H

#include <shell.h>

void cmd_nosecone(BaseSequentialStream *chp, int argc, char *argv[]);

extern const ShellCommand shell_commands[];

#endif /* COMMANDS_H */
