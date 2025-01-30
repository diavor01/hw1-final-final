#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "restoration.h"
#include "functions.h"
#include "readaline.h"
#include "seq.h"
#include "atom.h"
#include "unit_tests.h"

// // The lines are not supposed to end with \0 because then len is going to be 1 higher 


// int main()
// {
//     test_readaline_basic();
//     return 0;
// }
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
        // Argument provided: attempt to open the file
        input = fopen(argv[1], "rb");
        if (input == NULL) {
            perror("Error opening file");
            return EXIT_FAILURE;
        }
    } else {
        // No argument provided: use standard input
        input = stdin;
    }

    restoration(input);

    return 0;
}

//     const char* correct_atom = NULL;
//     char *line = NULL;
//     size_t len;
//     Seq_T matrix = Seq_new(0);
//     Seq_T atom_sequence = Seq_new(0);
//     int ok = 0;
//     int width = 0;
//     // int heigth = 0;
//     int false_lines = 0;
//     int total_corrupted_lines = 0;

//     while ((len = readaline(input, &line)) > 0) 
//     {
//         // printf("\n");
//         // printf("len: %zu", len);
//         // printf("\n");
//         // printf("Current line\n");
//         // printf("%s\n", line);
//         total_corrupted_lines++;
//         if (ok == 0)
//         {
//             int index = diff_nums_chars1(line, matrix, atom_sequence);
//             if (index != -1)
//             {
//                 // printf("The index is: ");
//                 // printf("%d", index);
//                 // printf("\n");

//                 correct_atom = (char*) Seq_get(atom_sequence, index);

//                 int size_matrix = Seq_length(matrix);
//                 false_lines = Seq_length(matrix) - 2;

//                 matrix = correct_matrix(matrix, size_matrix, index);

//                 //printf("Seq_length(Seq_get(matrix, 0)): %d\n", Seq_length(Seq_get(matrix, 0)));
//                 assert(Seq_length(Seq_get(matrix, 0)) == Seq_length(Seq_get(matrix, 1)));
//                 width = Seq_length(Seq_get(matrix, 0));

//                 //printf("%s", "The matrix after discovering the first correct 2 indices\n");
//                 //printing_matrix_to_file(matrix, "out.pgm");
//                 //printf("\n");

//                 ok = 1;
//             }
//             else
//             {
//                 //printf("Have not discovered the correct 2 indices yet\n");
//             }
//         }
//         else
//         {
//             false_lines+= diff_nums_chars2(line, correct_atom, matrix, atom_sequence, width);
//         }

//         free(line);
//     }

//     // printf("total_corrupted_lines: %d", total_corrupted_lines);
//     // printf("\n");
//     // printf("Matrix length: %d", Seq_length(matrix));
//     // printf("\n");
//     // printf("False lines: %d", false_lines);
//     // printf("\n");

//     assert(total_corrupted_lines - Seq_length(matrix) == false_lines);

//     if (input != stdin) {
//         fclose(input);
//     }

    
//     // printf("\n");
//     // printf("%s", "Final matrix\n");
//     printing_matrix_to_file(matrix, "out.pgm");

//     for (int i = 0; i < Seq_length(matrix); i++) {
//         Seq_T row = Seq_get(matrix, i);
//         // printf("\n");
//         // printf("Now working on row: %d\n", i);
//         for (int j = 0; j < Seq_length(row); j++) {
//             int *num = Seq_get(row, j);
//             free(num); 
//             // printf("Freed successfully for %d\n", j);
//         }

//         Seq_free(&row);
//         // printf("Freed successfully the row at %d\n", i);
//     }
//     Seq_free(&matrix);
//     //printf("Freed successfully the matrix\n");

//     // for (size_t j = 0; j < (size_t)Seq_length(atom_sequence); j++) {
//     //     char *atom = Seq_get(atom_sequence, j);
//     //     free(atom);
//     //     //printf("Freed successfully for atom at %zu\n", j);
//     // }
//     Seq_free(&atom_sequence);
//     // printf("Freed successfully the atom_sequence\n");

//     return 0;
// }









//     if (argc == 2)
//     {
//         FILE *input = fopen(argv[1], "rb");

//         if (!input) {
//             perror("fopen");
//             return 1;
//         }
//         const char* correct_atom = NULL;

//         char *line = NULL;
//         size_t len;
//         Seq_T matrix = Seq_new(0);
//         Seq_T atom_sequence = Seq_new(0);
//         int ok = 0;
//         int width = 0;
//         // int heigth = 0;
//         int false_lines = 0;
//         int total_corrupted_lines = 0;

//         while ((len = readaline(file, &line)) > 0) 
//         {
//             // printf("\n");
//             // printf("len: %zu", len);
//             // printf("\n");
//             // printf("Current line\n");
//             // printf("%s\n", line);
//             total_corrupted_lines++;
//             if (ok == 0)
//             {
//                 int index = diff_nums_chars1(line, matrix, atom_sequence);
//                 if (index != -1)
//                 {
//                     // printf("The index is: ");
//                     // printf("%d", index);
//                     // printf("\n");

//                     correct_atom = (char*) Seq_get(atom_sequence, index);

//                     int size_matrix = Seq_length(matrix);
//                     false_lines = Seq_length(matrix) - 2;

//                     matrix = correct_matrix(matrix, size_matrix, index);

//                     //printf("Seq_length(Seq_get(matrix, 0)): %d\n", Seq_length(Seq_get(matrix, 0)));
//                     assert(Seq_length(Seq_get(matrix, 0)) == Seq_length(Seq_get(matrix, 1)));
//                     width = Seq_length(Seq_get(matrix, 0));

//                     //printf("%s", "The matrix after discovering the first correct 2 indices\n");
//                     //printing_matrix_to_file(matrix, "out.pgm");
//                     //printf("\n");

//                     ok = 1;
//                 }
//                 else
//                 {
//                     //printf("Have not discovered the correct 2 indices yet\n");
//                 }
//             }
//             else
//             {
//                 false_lines+= diff_nums_chars2(line, correct_atom, matrix, atom_sequence, width);
//             }

//             free(line);
//         }

//         // printf("total_corrupted_lines: %d", total_corrupted_lines);
//         // printf("\n");
//         // printf("Matrix length: %d", Seq_length(matrix));
//         // printf("\n");
//         // printf("False lines: %d", false_lines);
//         // printf("\n");

//         assert(total_corrupted_lines - Seq_length(matrix) == false_lines);
//         fclose(file);
        
//         // printf("\n");
//         // printf("%s", "Final matrix\n");
//         printing_matrix_to_file(matrix, "out.pgm");

//         for (int i = 0; i < Seq_length(matrix); i++) {
//             Seq_T row = Seq_get(matrix, i);
//             // printf("\n");
//             // printf("Now working on row: %d\n", i);
//             for (int j = 0; j < Seq_length(row); j++) {
//                 int *num = Seq_get(row, j);
//                 free(num); 
//                 // printf("Freed successfully for %d\n", j);
//             }

//             Seq_free(&row);
//             // printf("Freed successfully the row at %d\n", i);
//         }
//         Seq_free(&matrix);
//         //printf("Freed successfully the matrix\n");

//         // for (size_t j = 0; j < (size_t)Seq_length(atom_sequence); j++) {
//         //     char *atom = Seq_get(atom_sequence, j);
//         //     free(atom);
//         //     //printf("Freed successfully for atom at %zu\n", j);
//         // }
//         Seq_free(&atom_sequence);
//         // printf("Freed successfully the atom_sequence\n");

//         return 0;
//     }
//     else
//     {
//         printf("%d", argc);
//         return 1;
//     }
// }


