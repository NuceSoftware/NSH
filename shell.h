//  shell.h
//  NSH
//
//  Created by Xenthio (Ethan Cardwell) on 17/8/20.

#ifndef shell_h
#define shell_h

#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "strings.h"


#endif /* shell_h */

// ----- some small settings ----- //
#define READBUFSIZE 1024
#define TOKENBUFSIZE 64
#define PATHBUFSIZE 64

#define TOKENDELIM " \t\r\n\a"
#define PATHDELIM "/" // How to split the paths, "/" in nix systems "\" in windows.

extern int SHELL_EXECUTE_CMD(char **args);
extern char *SHELL_READ_LINE(void);
extern char **SHELL_SPLIT_LINE(char *line);
extern void SHELL_PROMPT();
