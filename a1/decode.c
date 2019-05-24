#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 256

// Error messages 
#define USAGE "Usage: decode textfile codefile\n"
#define INVALID_LINE "Invalid line number\n"
#define EMPTY "Document is empty\n"
#define INVALID_POSITION "Invalid character position for this line\n"
#define LENGTH "Invalid chunk size"


int main(int argc, char *argv[]) {
    // If the user did not enter the correct number of arguments then exit
    if(argc != 3) { 
        fprintf(stderr, USAGE);
        exit(1); 
        // The argument of 1 is a convention indicating the program failed
    }

    FILE *tf; // File pointer for the text document
    FILE *cf; // File pointer for the code file

    /* Open both files for reading. Terminate if there is any problem 
     * opening the files */
    if((tf = fopen(argv[1], "r")) == NULL) {
        perror("fopen");
        exit(1);
    }

    if((cf = fopen(argv[2], "r")) == NULL) {
        perror("fopen");
        exit(1);
    }

    // We will assume that lines read from the text file and the code file
    // will never exceed MAXLINE-1 characters.
    char code_line[MAXLINE]; 
    char text_line[MAXLINE];
    
    // Add your implmeentation below.
    int current_line_num = -1;
    while (fgets(code_line, MAXLINE, cf) != NULL) {
        char *temp1;
        char *temp2;
        char *temp3;
        int line_num = strtol(code_line, &temp1, 10);
        int index = strtol(temp1, &temp2, 10);
        int word_length = strtol(temp2, &temp3, 10);
        if (current_line_num != line_num) {
            int run_time = line_num - current_line_num;
            current_line_num = line_num;
            for (int i = 0; i < run_time; i++) {
                if (fgets(text_line, MAXLINE, tf) == NULL) {
                    fprintf(stderr, INVALID_LINE);
                    exit(1);
                }
            }
        }
        int index_tester = 0;
        while (index_tester <= index){
            if (text_line[index_tester] != '\0') {
                index_tester++;
            } else {
                fprintf(stderr, INVALID_POSITION);
                exit(1);
            }
        }
        int length_tester = 0;
        while (length_tester < word_length){
            if (text_line[index + length_tester] != '\0') {
                printf("%c", text_line[index + length_tester]);
                length_tester++;
            } else {
                fprintf(stderr, LENGTH);
                exit(1);
            }
        }
    }
    

    // Print a newline after the end of the message so the prompt appears on 
    // the next line.
    printf("\n");
    return 0;
}
