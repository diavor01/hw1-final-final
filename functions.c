#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <assert.h>

#include "atom.h"
#include "seq.h"
#include "functions.h"


/*
 * name:      diff_nums_chars1(const char *line, Seq_T matrix, 
              Seq_T atom_sequence) (short for "differentiate between numbers and
              other characters version 1")
 * purpose:   using a helper function called separate, it differentiates
              between numbers and injected characters. The numbers are stored 
              inside a sequence called newRow, while the non-digit characters
              are stored using atoms. Atoms were used because their 
              implementation allows for easy comparrison between them, making 
              them ideal to detect equalities between injected sequences.
 * arguments: the line of text, a sequence of sequences of pointers to integers
              (converted to binary for the final p5 file), and a sequence of 
              atoms
 * returns:   -1 if it did not find any 2 equal atoms in atom_sequence or the 
 *            index at which the an identical atoms was found (chechk the 
 *            implementation of check_atoms for more details)
 * Author: Darius-Stefan Iavorschi
 */
int diff_nums_chars1(const char *line, Seq_T matrix, Seq_T atom_sequence)
{
    assert(line != NULL);

    int capacity = 1001;
    Seq_T newRow = Seq_new(capacity);
    char* atom_val = malloc(capacity);

    if (!atom_val) {
        perror("Failed to allocate memory for atom_val");
        exit(EXIT_FAILURE);
    }

    int size_atom = separate(line, newRow, atom_val);

    // printf("The value of atom_val is: %s\n", atom_val);

    Seq_addhi(matrix, newRow);

    const char *atom = Atom_new(atom_val, size_atom);

    // printf("The value of atom is: %s\n", atom);

    Seq_addhi(atom_sequence, (void *)atom);

    free(atom_val);

    return check_atoms(atom_sequence, atom);
}


/*
 * name:      diff_nums_chars2(const char* line, const char* correct_atom, Seq_T matrix, 
                                                Seq_T atom_sequence, int width) (short for "differentiate between numbers and
              other characters version 2")
 * purpose:   using a helper function called separate, it differentiates
              between numbers and injected characters. The difference between this function and diff_nums_chars1 is that 
 * arguments: the line of text, the verified injected sequence called correct_atom, the matrix containg the raw p5 data, and the known width of known original lines
 * returns:   fl (short for false lines, reffering to the non-orignal lines 
              inside the corrupted file). In the restoration function, the 
              program must check that the height of the matrix + the false 
              lines equals the total number of corrupted lines before constructing the the raw pgm file
 * Author: Darius-Stefan Iavorschi
 */
int diff_nums_chars2(const char* line, const char* correct_atom, Seq_T matrix, 
                                                 int width)
{
    assert(line != NULL);
    assert(correct_atom != NULL);

    int capacity = 1001;
    Seq_T newRow = Seq_new(capacity);
    char* atom_val = malloc(capacity);
    int fl = 0;


    if (!atom_val) {
        perror("Failed to allocate memory for atom_val");
        exit(EXIT_FAILURE);
    }

    /*First, we are just checking for atom equality. We are constructing the newRow only if that checks out*/
    int size_atom = construct_injected_sequence(line, atom_val);

    const char *atom = Atom_new(atom_val, size_atom);

    /* Veryfing whether the injected sequence is the same as the correct one 
    */ 
    if (atom == correct_atom)
    {
        // printf("Printing Seq_length(newRow): %d\n", Seq_length(newRow));
        // printf("The width is: %d\n", width);

        construct_newRow(line, newRow);

        /* Making sure the plain new row of data has the same width as
        * the other lines
        */  
        assert(Seq_length(newRow) == width);

        //print_sequence(newRow);
        /* Adding the data to the matrix
        */  
        Seq_addhi(matrix, writeRowToBinary(newRow, width));
    } else {
        fl++;
    }
        

    free(atom_val);

    /* Returning 0 or 1: 0 if the line was original, 1 if it was injected*/
    return fl;
}



/*
 * name:      separate(const char* line, Seq_T newRow, char* atom_val)
 * purpose:   a helper function for diff_nums_chars1.
 * arguments: the line of text, the seqence we want to append the numbers to,
 *            and char pointer atom_val that will be passed as an atom value
 *            back in diff_nums_chars1
 * returns:   the size of the atom, as it is a necessary argument for 
 *            constructing a new atom
 * Author: Darius-Stefan Iavorschi
 */
int separate(const char* line, Seq_T newRow, char* atom_val)
{
    int capacity = 1001;
    char ch;
    int size_atom = 0;

    while ((ch = *line++) != '\n')
    {
        if (isdigit((unsigned char)ch)) 
        {
            char *num = malloc(4);
            if (!num) {
                perror("Failed to allocate memory for num\n");
                exit(1);
            }

            num[0] = ch;
            int length = 1;

            while ((ch = *line++) != '\n')
            {
                if (isdigit((unsigned char)ch))
                {
                    num[length] = ch;
                    length++;
                    if (length >= 5) 
                    { /*It should not exceed 4 (3 digits 
                                        plus the null character)*/ 
                        printf("Number exceeds 3 digits: %s\n", num);
                        free(num);
                        exit(EXIT_FAILURE);
                    }
                }
                else
                {     
                    num[length] = '\0';                
                    if (size_atom >= capacity - 1) 
                    {  
                        fprintf(stderr, "atom_val buffer overflow\n");
                        exit(EXIT_FAILURE);
                    }
                    atom_val[size_atom++] = ch;
                    break;
                }
            }

            int *number_ptr = malloc(sizeof(*number_ptr));
            if (!number_ptr) {
                perror("Failed to allocate memory for number_ptr\n");
                free(num);
                exit(EXIT_FAILURE);
            }

            *number_ptr = atoi(num);
            free(num); 

            //printf("The number_ptr value is %d\n", *number_ptr);

            /* Making sure the value is between 0 and 255*/
            assert(-1 < *number_ptr);
            assert(*number_ptr < 256);

            Seq_addhi(newRow, number_ptr);

            if (ch == '\n') {
                break;
            }
        } 
        else 
        {   
            if (size_atom >= capacity - 1) 
            {  
                fprintf(stderr, "atom_val buffer overflow\n");
                exit(EXIT_FAILURE);
            }
            atom_val[size_atom++] = ch;
        }
    }
    atom_val[size_atom] = '\0';

    return size_atom;
}

/*
 * name:      construct_injected_sequence(const char* line, char* atom_val)
 * purpose:   a helper function for diff_nums_chars2. It has the same logic as separate without building a new row
 * arguments: the line of text,
 *            and char pointer atom_val that will be passed as an atom value
 *            back in diff_nums_chars2
 * returns:   the size of the atom, as it is a necessary argument for 
 *            constructing a new atom
 * Author: Darius-Stefan Iavorschi
 */
int construct_injected_sequence(const char* line, char* atom_val)
{
    int capacity = 1001;
    char ch;
    int size_atom = 0;

    while ((ch = *line++) != '\n')
    {
        if (isdigit((unsigned char)ch)) 
        {
            continue;
        } 
        else 
        {   
            if (size_atom >= capacity - 1) 
            {  
                fprintf(stderr, "atom_val buffer overflow\n");
                exit(EXIT_FAILURE);
            }
            atom_val[size_atom++] = ch;
        }
    }
    atom_val[size_atom] = '\0';

    return size_atom;
}


/*
 * name:      construct_newRow(const char* line, Seq_T newRow)
 * purpose:   a helper function for diff_nums_chars2. It has the same logic as separate, but it only constructs the newRow
 * arguments: the line of text, and the seqence we want to append the numbers to called newRow
 * returns:   nothing
 * Author: Darius-Stefan Iavorschi
 */
void construct_newRow(const char* line, Seq_T newRow)
{
    char ch;

    while ((ch = *line++) != '\n')
    {
        if (isdigit((unsigned char)ch)) 
        {
            char *num = malloc(4);
            if (!num) {
                perror("Failed to allocate memory for num\n");
                exit(EXIT_FAILURE);
            }

            num[0] = ch;
            int length = 1;

            while ((ch = *line++) != '\n')
            {
                if (isdigit((unsigned char)ch))
                {
                    num[length] = ch;
                    length++;
                    if (length >= 5) { 
                        printf("Number exceeds 3 digits: %s\n", num);
                        free(num);
                        exit(EXIT_FAILURE); 
                    }
                }
                else
                {     
                    num[length] = '\0';                
                    break;
                }
            }

            int *number_ptr = malloc(sizeof(*number_ptr));
            if (!number_ptr) {
                perror("Failed to allocate memory for number_ptr\n");
                free(num);
                exit(EXIT_FAILURE);
            }

            *number_ptr = atoi(num);
            free(num); 

            //printf("The number_ptr value is %d\n", *number_ptr);

            assert(-1 < *number_ptr);
            assert(*number_ptr < 256);

            Seq_addhi(newRow, number_ptr);

            if (ch == '\n') {
                break;
            }
        } 

    }
}

/*
 * name:      check_atoms(Seq_T atom_sequence, const char* atom)
 * purpose:   checks for atom equality inside the atom_sequence. The function becomes obsolete after discovering the first 2 identical injected sequences of characters
 * arguments: the atom_sequence and the constructed atom
 * returns:   the index at which the pair of identical atoms was found (the index of the other atom is already known: Seq_length(atom_sequence) - 1). If it could not be found, returns -1
 * Author: Darius-Stefan Iavorschi
 */
int check_atoms(Seq_T atom_sequence, const char* atom)
{
    
    for (int i = 0; i < Seq_length(atom_sequence) - 1; i++) {
        const char *current_atom = Seq_get(atom_sequence, i);
        // printf("\n");
        // printf("i: %d\n", i);
        // printf("%p\n", current_atom);
        // printf("%p\n", atom);
        if (current_atom == atom)
        {
            // printf("Found the indices\n");

            // printf("First index %d\n", i);

            // printf("Second index %d\n", Seq_length(atom_sequence));
            return i;
        }
            
    }
    // printf("returning %d", -1);
    return -1;
}


// Not finished yet
/*
 * name:      correct_matrix(Seq_T matrix, int size_matrix, int index)
 * purpose:   Constructs a new matrix with 
 * arguments: the atom_sequence and the constructed atom
 * returns:   the index at which the pair of identical atoms was found (the index of the other atom is already known: Seq_length(atom_sequence) - 1). If it could not be found, returns -1
 * Author: Darius-Stefan Iavorschi
 */
Seq_T correct_matrix(Seq_T matrix, int size_matrix, int index, int width)
{
    // 1. Build the new matrix with the two “correct” rows
    Seq_T new_matrix = Seq_seq(writeRowToBinary(Seq_get(matrix, index), width), 
                        writeRowToBinary(Seq_get(matrix, size_matrix - 1), width),
                        NULL);

    // print_sequence(Seq_get(matrix, index));
    // print_sequence(Seq_get(matrix, size_matrix - 1));
    // 2. Free all other rows
    //    (Don’t free the rows at index and size_matrix-1 
    //     because they're in new_matrix)
    for (int i = 0; i < size_matrix; i++) {
        if (i != index && i != size_matrix - 1) {
            Seq_T row = Seq_get(matrix, i);
            int length = Seq_length(row);
            for (int j = 0; j < length; j++) {
                int *number_ptr = Seq_get(row, j);
                free(number_ptr); 
            }
            Seq_free(&row);
        }
    }

    // 3. Now free the old matrix
    Seq_free(&matrix);

    // 4. Return the new one
    return new_matrix;
}

//Creating the P5 value
void printing_matrix(Seq_T matrix, int width, int height)
{
    // Print the P5 header
    printf("P5\n");
    printf("%d %d\n", width, height);
    printf("255\n");

    // For each row in the matrix
    for (int row = 0; row < height; row++) {
        // Get the row sequence of bytes
        Seq_T row_seq = Seq_get(matrix, row);

        // Print each byte to stdout
        for (int col = 0; col < width; col++) {
            uint8_t *pixel = Seq_get(row_seq, col);
            // fputc or putchar both write a single byte to stdout
            fputc(*pixel, stdout);
        }
    }
}

// for (int row = 0; row < height; row++) {
//         Seq_T byte_seq = (Seq_T)Seq_get(matrix, row);

//         for (int col = 0; col < width; col++) {
//             // Each element in byte_seq is an unsigned char*, so retrieve it:
//             unsigned char *byte_ptr = Seq_get(byte_seq, col);
//             if (byte_ptr) {
//                 putchar(*byte_ptr);
//             } else {

//                 fprintf(stderr, "Could not read byte_ptr");
//             }
//         }
//     }

// Function to write a row of pixels to binary file
Seq_T writeRowToBinary(Seq_T seq, int width)
{
    if (seq == NULL) {
        fprintf(stderr, "Error: Invalid input to writeRowToBinary\n");
        return NULL;
    }

    // Create a new sequence to hold the 8-bit data
    Seq_T byte_seq = Seq_new(width);

    for (int i = 0; i < width; i++) {
        int gray_value = *(int *)Seq_get(seq, i);

        // Allocate a single byte on the heap (important to match Seq_T usage)
        uint8_t *pixel_byte = malloc(sizeof(*pixel_byte));
        if (!pixel_byte) {
            fprintf(stderr, "Error: Could not allocate memory for pixel\n");
            Seq_free(&byte_seq);
            return NULL;
        }
        *pixel_byte = (uint8_t)gray_value;

        // Add it to the new sequence
        Seq_addhi(byte_seq, pixel_byte);
    }

    return byte_seq;
}


// int value = (int)(intptr_t)Seq_get(seq, i);

//         unsigned char byte_value = (unsigned char)value;

//         /* Allocate storage for one byte and store it */
//         unsigned char *stored_val = malloc(sizeof(*stored_val));
//         *stored_val = byte_value;

//         /* Add pointer to that single byte to the sequence */
//         Seq_addhi(byte_seq, stored_val);

void print_sequence(Seq_T sequence) {
    for (int i = 0; i < Seq_length(sequence); i++) {
        int* value = Seq_get(sequence, i);  // Correctly cast the value
        printf("%d ", *value);
    }
    printf("\n");
}


// void printing_atom_seq(Seq_T atom_sequence)
// {
//     printf("Printing the atom_sequence: \n");
//     for (size_t j = 0; j < (size_t)Seq_length(atom_sequence); j++) 
//     {
//         char *str = Seq_get(atom_sequence, j);
//         if (str) {
//             printf("%s", str);
//         } else {
//             printf("NULL ");
//         }
//     }
//     printf("\n");
// }
// void writeP5Header(const char *filename, int width, int height) {
//     assert(width > 0);
//     assert(height > 0);
//     assert(filename != NULL);

//     FILE *file = fopen(filename, "rb+");
//     if (!file) {
//         perror("Failed to open file");
//         exit(EXIT_FAILURE);
//     }

//     // Move the existing contents to make room for the header
//     fseek(file, 0, SEEK_END);
//     long fileSize = ftell(file);
//     char *buffer = (char *)malloc(fileSize);
//     if (buffer == NULL) {
//         perror("Memory allocation error");
//         fclose(file);
//         exit(EXIT_FAILURE);
//     }

//     fseek(file, 0, SEEK_SET);
//     fread(buffer, 1, fileSize, file);

//     fseek(file, 0, SEEK_SET);

//     fprintf(file, "P5\n%d %d\n255\n", width, height);
//     fwrite(buffer, 1, fileSize, file);

//     free(buffer);
//     fclose(file);
// }
// Function to trim leading and trailing whitespace from a string
// void trim_whitespace(char *str) {
//     char *end;

//     // Trim leading space
//     while (isspace((unsigned char)*str)) str++;

//     if (*str == 0) { // All spaces?
//         return;
//     }

//     // Trim trailing space
//     end = str + strlen(str) - 1;
//     while (end > str && isspace((unsigned char)*end)) end--;

//     // Write new null terminator
//     end[1] = '\0';
// }

// Function to convert a line into a sequence of numbers
// Seq_T convert_line_to_sequence(const char *line) {
//     if (line == NULL) {
//         return NULL;
//     }

//     Seq_T sequence = Seq_new(0);
//     if (sequence == NULL) {
//         return NULL;
//     }

//     // Calculate length and allocate memory for copy
//     size_t len = strlen(line) + 1;
//     char *copy = malloc(len);
//     if (copy == NULL) {
//         Seq_free(&sequence);
//         return NULL;
//     }

//     // Copy the input line to avoid modifying the original string
//     strcpy(copy, line);

//     // Use sscanf to read numbers directly from the string
//     char *ptr = copy;
//     int number;
//     while (sscanf(ptr, "%d", &number) == 1) {
//         Seq_addhi(sequence, (void *)(intptr_t)number);

//         // Move ptr to the next non-whitespace character after the last number
//         while (*ptr && !isspace((unsigned char)*ptr)) ptr++;
//         while (*ptr && isspace((unsigned char)*ptr)) ptr++;
//     }

//     // Free the copied string memory
//     free(copy);

//     return sequence;
// }


