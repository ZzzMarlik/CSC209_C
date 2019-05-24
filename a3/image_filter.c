#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "bitmap.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define ERROR_MESSAGE "Warning: one or more filter had an error, so the output image may not be correct.\n"
#define SUCCESS_MESSAGE "Image transformed successfully!\n"


/*
 * Check whether the given command is a valid image filter, and if so,
 * run the process.
 *
 * We've given you this function to illustrate the expected command-line
 * arguments for image_filter. No further error-checking is required for
 * the child processes.
 */
void run_command(const char *cmd) {
    if (strcmp(cmd, "copy") == 0 || strcmp(cmd, "./copy") == 0 ||
        strcmp(cmd, "greyscale") == 0 || strcmp(cmd, "./greyscale") == 0 ||
        strcmp(cmd, "gaussian_blur") == 0 || strcmp(cmd, "./gaussian_blur") == 0 ||
        strcmp(cmd, "edge_detection") == 0 || strcmp(cmd, "./edge_detection") == 0) {
        execl(cmd, cmd, NULL);
    } else if (strncmp(cmd, "scale", 5) == 0) {
        // Note: the numeric argument starts at cmd[6]
        execl("scale", "scale", cmd + 6, NULL);
    } else if (strncmp(cmd, "./scale", 7) == 0) {
        // Note: the numeric argument starts at cmd[8]
        execl("./scale", "./scale", cmd + 8, NULL);
    } else {
        fprintf(stderr, "Invalid command '%s'\n", cmd);
        exit(1);
    }
}


// TODO: Complete this function.
int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Usage: image_filter input output [filter ...]\n");
        exit(1);
    }

    // Set up the redirections
    int image = open(argv[1], O_RDONLY);
    int result = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);

    // Reset stdin so that when we read from stdin it comes from the file
    if ((dup2(image, fileno(stdin))) == -1) {
        perror("dup2");
        exit(1);
    }
    // Reset stdout so that when we write to stdout it goes to the target file
    if ((dup2(result, fileno(stdout))) == -1) {
        perror("dup2");
        exit(1);
    }


    if (argc < 5) { // one filter case

        int r;

        r = fork();

        if (r == 0) {
            if (argc == 3) {
                run_command("./copy");
                fprintf(stderr, "ERROR: exec should not return\n");
            }
            else {
                run_command(argv[argc - 1]);
                fprintf(stderr, "ERROR: exec should not return\n");
            }
        } else if (r > 0) {
            int status;
            if (wait(&status) != -1) {
                if (WIFEXITED(status)) {
                    int check = WEXITSTATUS(status);
                    if (check == 0){
                        fprintf(stderr, "%s", SUCCESS_MESSAGE);
                    } else {
                        fprintf(stderr, "%s", ERROR_MESSAGE);
                    }
                } else {
                    fprintf(stderr, "Process teminated\n");
                }
            }

        } else {
            perror("fork");
            exit(1);
        }
    } else { //handle multiple filters case
        int fd[argc - 3][2], r;
        for (int i = 0; i < argc - 3; i++) {
            if ((pipe(fd[i])) == -1) {
                perror("pipe");
                exit(1);
            }
            if ((r = fork()) == 0) {
                if (i == 0) { // first filter
                    if ((dup2(fd[i][1], fileno(stdout))) == -1) {
                        perror("dup2");
                        exit(1);
                    }
                    close(fd[i][1]);
                    close(fd[i][0]);
                    run_command(argv[i + 3]);
                } else if (i > 0 && i < argc - 4) { // middle filter
                    if ((dup2(fd[i - 1][0], fileno(stdin))) == -1) {
                        perror("dup2");
                        exit(1);
                    }
                    if ((dup2(fd[i][1], fileno(stdout))) == -1) {
                        perror("dup2");
                        exit(1);
                    }
                    for (int j = 0; j <= i; j++) {
                        close(fd[j][0]);
                        close(fd[j][1]);
                    }
                    run_command(argv[i + 3]);
                } else { // last filter
                    if ((dup2(fd[i - 1][0], fileno(stdin))) == -1) {
                        perror("dup2");
                        exit(1);
                    }
                    for (int j = 0; j <= i; j++) {
                        close(fd[j][0]);
                        close(fd[j][1]);
                    }
                    run_command(argv[argc - 1]);
                }
            } else if (r < 0) {
                perror("fork");
                exit(1);
            }
        }

        for (int a = 0; a <= argc - 4; a++) {
            close(fd[a][0]);
            close(fd[a][1]);
        }
        int status;
        for (int b = 0; b <= argc - 4; b++) {
            if (wait(&status) != -1) {
                if (WIFEXITED(status)) {
                    int check = WEXITSTATUS(status);
                    if (check == 1) {
                        fprintf(stderr, "%s", ERROR_MESSAGE);
                    }
                }
            }
        }
        fprintf(stderr, "%s", SUCCESS_MESSAGE);
    }
    return 0;
}
