#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minishell.h"

#define BUFFER_SIZE 4096

// Function to read a line from file
char *get_next_line(FILE *file) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    read = getline(&line, &len, file);
    if (read == -1) {
        free(line);
        return NULL;
    }
    
    // Remove newline character if present
    if (read > 0 && line[read - 1] == '\n') {
        line[read - 1] = '\0';
    }
    
    return line;
}

int main(int argc, char **argv, char **envp) {
    FILE *test_file;
    char *line;
    t_tree *tree;
    int error;

	//Para silenciar warning
	if (argc != 1 || !argv)
		return(0);

    // Open test file
    test_file = fopen("test_menos.txt", "r");
    if (!test_file) {
        perror("Error opening test file");
        return 1;
    }

    error = 0;
    while (error == 0) {
        line = get_next_line(test_file);
        if (!line) {
            // End of file or error
            break;
        }

        tree = processline(line);
        if (tree == NULL) {
            perror("processline:");
            free(line);
            fclose(test_file);
            return 1;
        }

        if (process_tree(tree, envp)) {
            perror("62->expandtree:");
        }
		print_tree(tree, 30);
       // error = execute(tree, envp);
        free(line);
        free_tree(tree);
    }

    fclose(test_file);
    return error;
}