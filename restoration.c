#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "functions.h"
#include "readaline.h"
#include "seq.h"
#include "atom.h"
#include "unit_tests.h"


/*
 * name:      restoration(FILE *input)
 * purpose:   the orchestrator of the program
 * arguments: the input (which can be a file or stdin)
 * effects:   creates a p5 pgm files out of a corrupted p2 pgm file
 * author: Darius-Stefan Iavorschi
 */
void restoration(FILE *input)
{
    if (input == NULL) {
        fprintf(stderr, "Error: Input file is NULL. Exiting function.\n");
        exit(EXIT_FAILURE);
    }

    const char* correct_atom = NULL;
    char *line = NULL;
    size_t len;
    Seq_T matrix = Seq_new(0);
    Seq_T atom_sequence = Seq_new(0);
    int ok = 0;
    int width = 0;
    int heigth = 0;
    int false_lines = 0;
    int total_corrupted_lines = 0;

    while ((len = readaline(input, &line)) > 0) 
    {
        total_corrupted_lines++;
        if (ok == 0)
        {
            int index = diff_nums_chars1(
                line, 
                matrix, 
                atom_sequence
            );
            
            if (index != -1)
            {
                correct_atom = (char*) Seq_get(atom_sequence, index);

                int size_original_matrix = Seq_length(matrix);
                false_lines = Seq_length(matrix) - 2;

                width = Seq_length(Seq_get(matrix, index));

                matrix = correct_matrix(
                    matrix, 
                    size_original_matrix, 
                    index, 
                    width
                );
                assert(Seq_length(Seq_get(matrix, 0)) 
                == Seq_length(Seq_get(matrix, 1)));

                ok = 1;
            }
        }
        else
        {
            false_lines+= diff_nums_chars2(
                line, 
                correct_atom, 
                matrix, 
                width
            );
        }

        free(line);
    }
    heigth = Seq_length(matrix);

    assert(total_corrupted_lines - heigth == false_lines);

    if (input != stdin) {
        fclose(input);
    }
    printing_matrix(matrix, width, heigth);

    for (int i = 0; i < Seq_length(matrix); i++) {
        Seq_T row = Seq_get(matrix, i);
        for (int j = 0; j < Seq_length(row); j++) {
            int *num = Seq_get(row, j);
            free(num); 
        }

        Seq_free(&row);
    }
    Seq_free(&matrix);
    Seq_free(&atom_sequence);
}

