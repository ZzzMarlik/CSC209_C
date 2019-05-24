#include <stdio.h>
#include <stdlib.h>

#include <string.h>  // Only used for strlen


#define MAXLINE 256

// Error messages
#define USAGE "Usage: encode chunksize textfile msg\n"
#define ENCODE_FAILED "Could not encode message.\n"


int main(int argc, char *argv[]) {
    // Check to make sure the user entered the correct number of arguments
    if(argc != 4) { 
        fprintf(stderr, USAGE);
        exit(1); 
        // The argument of 1 is a convention indicating the program failed
    }

    FILE *tf; // File pointer for the text document

    // determine message chunks size
    int chunk_size = strtol(argv[1], NULL, 0);
    if(chunk_size == 0) {
        fprintf(stderr, "Invalid chunk size\n");
        exit(1);
    }

    if((tf = fopen(argv[2], "r")) == NULL) {
        perror("fopen");
        exit(1);
    }
    
    // The message to encode is in argv[3].  We will use a pointer to char
    // to give it a better name.
    char *message = argv[3];
    int msg_length = strlen(message);  // only allowed use of string functions
    
    // Complete the implmentation below
    char text_line[MAXLINE];
    int line_num = -1;
    int answer_index = 0;
    int num_result;
    if ((msg_length % chunk_size) == 0) {
        num_result = msg_length/chunk_size;
    } else {
        num_result = msg_length/chunk_size + 1;
    }
    int found_chunk = 0;
    while (found_chunk < num_result) {
        if (fgets(text_line, MAXLINE, tf) == NULL) {
            fprintf(stderr, ENCODE_FAILED);
            exit(1);
        }
        line_num++; //update line number after fgets
        int index = 0;
        int found_flag = 0; //use int as boolean flag
        // check whether the current size that need to match is still same as given chunk_size
        int current_size;
        if (answer_index + chunk_size <= msg_length) {
            current_size = chunk_size;
        } else {
            current_size = msg_length % chunk_size;
        }
        //start to search the chunk in this line
        while ((text_line[index] != '\0') & (found_flag != 1)) {
            if (text_line[index] == message[answer_index]) {
                int num_match = 1;
                int run_time = current_size - num_match;
                for (int a = 0; a < run_time; a++) {
                    if (text_line[index + num_match] == message[answer_index + num_match]) {
                        num_match++;
                    }
                }
                if (num_match == current_size) {
                    found_flag = 1;
                    printf("%i %i %i\n", line_num, index, current_size);
                    answer_index += current_size;
                    found_chunk++;
                } else {
                    index++;
                }
            } else {
                index++;
            }
        }

    }


    return 0;
}
