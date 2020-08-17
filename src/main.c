//  main.c
//  NSH
//
//  Created by Xenthio (Ethan Cardwell) on 17/8/20.

#include <stdio.h>
#include "shell.h"

int main(int argc, const char * argv[]) {
	
	char *line;
	char **args;
	int stat;
	char * home = getenv("HOME");
	

	
	// Load config
	FILE * fp;
	char * rline = NULL;
    size_t rlen = 0;
	ssize_t read;
	
	fp = fopen(strcat(home, "/.nshrc"), "r");
	
	while ((read = getline(&rline, &rlen, fp)) != -1) {
		args = SHELL_SPLIT_LINE(rline);
		stat = SHELL_EXECUTE_CMD(args);
    }
    fclose(fp);
    if (rline)
        free(rline);
	
	// Execute main stuff
	do {
		SHELL_PROMPT();
		line = SHELL_READ_LINE();
		args = SHELL_SPLIT_LINE(line);
		stat = SHELL_EXECUTE_CMD(args);
		
		free(line);
		free(args);
	} while (stat);
	
	// Perform any shutdown/cleanup.
	
	return 1;

}
