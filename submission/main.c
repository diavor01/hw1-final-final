/*
 * main.c
 * Darius-Stefan Iavorschi
 * Alijah Jackson
 * 1/30/2025
 * CS 40 filesofpix
 * 
 * This file takes the input for the restoration program
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "restoration.h"
#include "functions.h"
#include "readaline.h"
#include "seq.h"
#include "atom.h"

/*
 * name:      main(int argc, char *argv[])
 * purpose:   established whether the program reads from a file or from stdin
 * arguments: the number of arguments and and the arguments themselves
 * effects:   executes the program logic using the function restoration
 */
int main(int argc, char *argv[]) 
{
    FILE *input;

    if (argc > 2) {
        fprintf(stderr, "Usage: %s [corrupted_pgm_file]\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (argc == 2) {
        input = fopen(argv[1], "rb");
        if (input == NULL) {
            perror("Error opening file");
            return EXIT_FAILURE;
        }
    } else {
        input = stdin;
    }

    restoration(input);

    return 0;
}
