#include <stdio.h>
#include <stdlib.h>
#include "readaline.h"

#include <stdio.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 1001

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

        // if (length >= capacity) {
        //     size_t new_capacity = capacity * 2;
        //     unsigned char *temp = realloc(buffer, new_capacity);
        //     if (!temp) {
        //         free(buffer);
        //         fprintf(stderr, "readaline: memory (re)allocation failed\n");
        //         exit(EXIT_FAILURE);
        //     }
        //     buffer = temp;
        //     capacity = new_capacity;
        // }

// char* increase_cap (char* buffer, size_t capacity)
// {
//     char* new_buff = realloc(buffer, capacity);

//     if (new_buff == NULL) {
//         fprintf(stderr, "Memory reallocation failed\n");
//         free(buffer);
//         exit(1);
//     }

//     return new_buff;
// }

