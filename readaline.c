#include <stdio.h>
#include <stdlib.h>
#include "readaline.h"

#include <stdio.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 1001


/*
 * name:      readaline(FILE *inputfd, char **datapp)
 * purpose:   Itterates through a file line by line and takes the call by reference variable dataapp and changes its value to that line or null
 * arguments: It takes the open file and a double pointer to the string line
 * returns:   It returns the length of the line or 0 if it reached the end of the file
 * Author: Darius-Stefan Iavorschi && Alijah Jackson
 */
size_t readaline(FILE *inputfd, char **datapp) {
    if (inputfd == NULL || datapp == NULL) {
        fprintf(stderr, "readaline: invalid argument\n");
        exit(EXIT_FAILURE);
    }

    size_t capacity = INITIAL_CAPACITY;
    unsigned char *buffer = malloc(capacity);

    if (!buffer) {
        fprintf(stderr, "readaline: memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    size_t length = 0;
    int ch;

    while ( (ch = fgetc(inputfd)) != EOF ) {

        if (length > 1000)
        {
            fprintf(stderr, "readaline: input line too long\n");
            free(buffer);
            exit(4);
        }
    
    
        buffer[length] = (unsigned char)ch;
        length++;

        if (ch == '\n') {
            break;
        }
    }

    if (ch == EOF && ferror(inputfd)) {
        fprintf(stderr, "readaline: error reading from file\n");
        free(buffer);
        exit(EXIT_FAILURE);
    }

    if (length == 0) 
    {
        free(buffer);
        *datapp = NULL;
        return 0;
    }

    *datapp = (char *)buffer;
    return length;
}

