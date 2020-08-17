//  shell.c
//  NSH
//
//  Created by Xenthio (Ethan Cardwell) on 17/8/20.

#include "shell.h"


/*
 Function Declarations for builtin shell commands:
 */
int SHELL_CD(char **args);
int SHELL_HELP(char **args);
int SHELL_EXIT(char **args);

/*
 List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

int (*builtin_func[]) (char **) = {
    &SHELL_CD,
    &SHELL_HELP,
    &SHELL_EXIT
};

int NUM_BUILTINS() {
    return sizeof(builtin_str) / sizeof(char *);
}

/*
 Builtin function implementations.
 */

/**
 @brief Bultin command: change directory.
 @param args List of args.  args[0] is "cd".  args[1] is the directory.
 @return Always returns 1, to continue executing.
 */
int SHELL_CD(char **args)
{
    if (args[1] == NULL) {
        fprintf(stderr, ERR_EXPARG);
    } else {
        if (chdir(args[1]) != 0) {
            perror(ERR_EXPARG);
        }
    }
    return 1;
}

/**
 @brief Builtin command: print help.
 @param args List of args.  Not examined.
 @return Always returns 1, to continue executing.
 */
int SHELL_HELP(char **args)
{
    int i;
    printf("XSH\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");
    
    for (i = 0; i < NUM_BUILTINS(); i++) {
        printf("  %s\n", builtin_str[i]);
    }
    
    printf("Use the man command for information on other programs.\n");
    return 1;
}

/**
 @brief Builtin command: exit.
 @param args List of args.  Not examined.
 @return Always returns 0, to terminate execution.
 */
int SHELL_EXIT(char **args)
{
    return 0;
}

/**
 @brief Launch a program and wait for it to terminate.
 @param args Null terminated list of arguments (including program).
 @return Always returns 1, to continue execution.
 */
int SHELL_LAUNCH(char **args)
{
    pid_t pid;
    int status;
    
    pid = fork();
    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            printf(ERR_INVCMD, args[0]);
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error forking
        printf(ERR_INVCMD, args[0]);
    } else {
        // Parent process
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    
    return 1;
}

/**
 @brief Execute shell built-in or launch program.
 @param args Null terminated list of arguments.
 @return 1 if the shell should continue running, 0 if it should terminate
 */
int SHELL_EXECUTE_CMD(char **args)
{
    int i;
    
    if (args[0] == NULL) {
        fprintf(stderr, ERR_EMPCMD);
        return 1;
    }
    
    for (i = 0; i < NUM_BUILTINS(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }
    
    return SHELL_LAUNCH(args);
}


/**
 @brief Read a line of input from stdin.
 @return The line from stdin.
 */
char *SHELL_READ_LINE(void)
{
    int bufsize = READBUFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;
    
    if (!buffer) {
        fprintf(stderr, ERR_MALLOC);
        exit(EXIT_FAILURE);
    }
    
    while (1) {
        // Read a character
        c = getchar();
        
        if (c == EOF) {
            exit(EXIT_SUCCESS);
        } else if (c == '\n') {
            buffer[position] = '\0';
            return buffer;
        } else {
            buffer[position] = c;
        }
        position++;
        
        // If we have exceeded the buffer, reallocate.
        if (position >= bufsize) {
            bufsize += READBUFSIZE;
            buffer = realloc(buffer, bufsize);
            if (!buffer) {
                fprintf(stderr, ERR_MALLOC);
                exit(EXIT_FAILURE);
            }
        }
    }
}


/**
 @brief Split a line into tokens (very naively).
 @param line The line.
 @return Null-terminated array of tokens.
 */
char **SHELL_SPLIT_LINE(char *line)
{
    int bufsize = TOKENBUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token, **tokens_backup;
    
    if (!tokens) {
        fprintf(stderr, ERR_MALLOC);
        exit(EXIT_FAILURE);
    }
    
    token = strtok(line, TOKENDELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;
        
        if (position >= bufsize) {
            bufsize += TOKENBUFSIZE;
            tokens_backup = tokens;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                free(tokens_backup);
                fprintf(stderr, ERR_MALLOC);
                exit(EXIT_FAILURE);
            }
        }
        
        token = strtok(NULL, TOKENDELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

char *SHELL_TOP_FROM_PATH(char *line)
{
    int bufsize = PATHBUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token, **tokens_backup;
    
    if (!tokens) {
        fprintf(stderr, ERR_MALLOC);
        exit(EXIT_FAILURE);
    }
    
    token = strtok(line, PATHDELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;
        
        if (position >= bufsize) {
            bufsize += PATHBUFSIZE;
            tokens_backup = tokens;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                free(tokens_backup);
                fprintf(stderr, ERR_MALLOC);
                exit(EXIT_FAILURE);
            }
        }
        
        token = strtok(NULL, TOKENDELIM);
    }
    tokens[position] = NULL;
    return tokens[position - 1];
}

void SHELL_PROMPT() {
	//printf(prompt);
	int MODE = 0;
	int PASS = 0;
	char clrCode[3];
	int pos = 0;
	char hostname[1024];
	hostname[1023] = '\0';
	gethostname(hostname, 1023);
	char * prompt = getenv("PROMPT");
	size_t i = 0;
	while (prompt[i] != '\0') {
		if (prompt[i] == '%') {
			//printf("-MARKER1-");
			MODE = 1;
		} else if (MODE == 1) {
			switch(prompt[i]) {
					case 'b':
						printf("\033[0m");
						break;
					case 'B':
						printf("\033[1m");
						break;
					case 'F':
						PASS = 1;
						break;
					case 'm':
						printf("%s", hostname);
						break;
					case '#':
						printf("%%");
						break;
					default:
						//printf("-MARKER2-");
						break;
			}
			
			MODE = 0;
		} else if (PASS == 1) {
			if (prompt[i] == '}') {
				PASS = 0;
				printf("\033[38;5;%sm", clrCode);
				
			} else if (prompt[i] != '{') {
				clrCode[pos] = prompt[i];
				pos++;
			}
		} else {
			printf("%c", prompt[i]);    /* Print each character of the string. */
		}
		i++;
	}
}
