// #include <unistd.h>  // Add this for `fork()` and `pid_t`
// #include <sys/types.h>  // For `pid_t`
// #include <sys/wait.h>   // For `waitpid()`
// #include <stdio.h>
// #include <stdlib.h>
// #include <assert.h>
// #include <string.h>

// #include "restoration.h"
// #include "readaline.h"
// #include "seq.h"

// void test_check_atoms() {
//     // Create a new sequence
//     Seq_T atom_sequence = Seq_new(7);

//     // Add atoms to the sequence
//     const char *atom1 = Atom_new("Zn", 2);
//     const char *atom2 = Atom_new("Cu", 2);
//     const char *atom3 = Atom_new("Ni", 2);

//     Seq_addhi(atom_sequence, (void *)atom1);
//     Seq_addhi(atom_sequence, (void *)atom2);
//     Seq_addhi(atom_sequence, (void *)atom3);


//     // Check for atom that does not exist
//     const char *atom4 = Atom_new("Mg", 2);
//     assert(check_atoms(atom_sequence, atom4) == -1);
//     Seq_addhi(atom_sequence, (void *)atom4);

//     printf("Sequence length: %d\n", Seq_length(atom_sequence));
//     for (int i = 0; i < Seq_length(atom_sequence); i++) {
//         printf("Seq[%d]: %p\n", i, (void *)Seq_get(atom_sequence, i));
//     }


//     // Check for atom that does exist
//     const char *atom5 = Atom_new("Mg", 2);
//     Seq_addhi(atom_sequence, (void *)atom5);

//     printf("Atom 5: %p\n", (void *)atom5);

//     // printf("%d\n", check_atoms(atom_sequence, atom5));

//     assert(check_atoms(atom_sequence, atom5) == 3);    

//     // Cleanup
//     Seq_free(&atom_sequence);

//     // If all assertions pass
//     printf("All tests passed.\n");
// }

// void test_diff_nums_chars1()
// {
//     const char* line1 = "242g‘119i176‚Y~71Ôï253ÏuP125¸202O58’117ï17ø^12g147+g152E——103¤ô223Š132óö2ý121Pú158îR221Rà‡249í,Ì¢183Åõ122ª128š§214Ô@63210oa39˜235à212Œ189fÒ221‡76Ñ¡109Î²36µÓ73¡234i239 132Ÿ96â0­÷144I243152š247Øµ211‡:28N249•76Ï186215w¢69ŸIÀ114n81ñwB198]_";
//     Seq_T matrix = Seq_new(0);
//     Seq_T atom_sequence = Seq_new(0);


//     diff_nums_chars1(line1, matrix, atom_sequence);
//     printf("Printing the number matrix\n");
//     printing_matrix_to_file(matrix, "out.pgm");
//     printf("Printing the atom_sequence\n");
//     printing_atom_seq(atom_sequence);
//     printf("\n");
// }

// void test_separate_extended()
// {
//     struct {
//         const char* line;
//         int expected_atom_len;
//         const char* expected_atom;
//         int expected_seq_vals[10];  // Assuming max 10 numbers for simplicity
//         int expected_seq_count;
//     } test_cases[] = {
//         {"123\n", 0, "", {123}, 1},
//         {"hello\n", 5, "hello", {0}, 0},
//         {"a1b2c3\n", 3, "abc", {1, 2, 3}, 3},
//         {"x100y200z255\n", 3, "xyz", {100, 200, 255}, 3},
//         {"   42  \n", 5, "     ", {42}, 1},
//         {"\0\0\0\n", 3, "\0\0\0", {0}, 0},
//         {"A.\0he123l\0oo245\0m10n\n", 12, "A.\0hel\0oo\0mn", {123, 245, 10}, 3}
//     };

//     for (size_t i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++)
//     {
//         printf("\n");
//         printf("Running Test Case %zu...\n", i + 1);

//         char* atom_val = malloc(100);
//         if (!atom_val) {
//             perror("Failed to allocate memory for atom_val");
//             exit(1);
//         }

//         Seq_T newRow = Seq_new(0);
//         int len = separate(test_cases[i].line, newRow, atom_val);

//         printf("len: %d\n", len);
//         printf("test_cases[i].expected_atom_len: %d\n\n", test_cases[i].expected_atom_len);

//         assert(len == test_cases[i].expected_atom_len);
//         assert(memcmp(atom_val, test_cases[i].expected_atom, len) == 0);

//         printf("Seq_length(newRow): %d\n", Seq_length(newRow));
//         printf("test_cases[i].expected_seq_count: %d\n", test_cases[i].expected_seq_count);

//         assert(Seq_length(newRow) == test_cases[i].expected_seq_count);
//         for (int j = 0; j < test_cases[i].expected_seq_count; j++)
//         {
//             int *num = Seq_get(newRow, j);
//             assert(num && *num == test_cases[i].expected_seq_vals[j]);
//             free(num);  // Free dynamically allocated numbers
//         }

//         free(atom_val);
//         Seq_free(&newRow);

//         printf("Test Case %zu Passed!\n", i + 1);
//         printf("\n");
//     }
// }


// void test_construct_newRow()
// {
//     struct {
//         const char* line;
//         int expected_seq_vals[10];  // Assuming max 10 numbers for simplicity
//         int expected_seq_count;
//     } test_cases[] = {
//         {"123\n", {123}, 1},
//         {"hello\n", {0}, 0},
//         {"a1b2c3\n", {1, 2, 3}, 3},
//         {"x100y200z255\n", {100, 200, 255}, 3},
//         {"   42  \n", {42}, 1},
//         {"\0\0\0\n", {0}, 0},
//         {"A.\0he123l\0oo245\0m10n\n", {123, 245, 10}, 3}
//     };

//     for (size_t i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++)
//     {
//         printf("\n");
//         printf("Running Test Case %zu...\n", i + 1);

//         char* atom_val = malloc(100);
//         if (!atom_val) {
//             perror("Failed to allocate memory for atom_val");
//             exit(1);
//         }

//         Seq_T newRow = Seq_new(0);
        
//         construct_newRow(test_cases[i].line, newRow);

//         assert(Seq_length(newRow) == test_cases[i].expected_seq_count);
//         for (int j = 0; j < test_cases[i].expected_seq_count; j++)
//         {
//             int *num = Seq_get(newRow, j);
//             assert(num && *num == test_cases[i].expected_seq_vals[j]);
//             free(num);  // Free dynamically allocated numbers
//         }

//         Seq_free(&newRow);

//         printf("Test Case %zu Passed!\n", i + 1);
//         printf("\n");
//     }
// }


// #define TEST_STRING_1 "Hello, World!\n"
// #define TEST_STRING_2 "Line 2 with some text.\n"
// #define TEST_STRING_3 "Hell\0\0Wor\0d!\n"
// #define TEST_STRING_4 "255 255 255 255 255 255 255 255 255 247 175 100 114 120 121 128 134 138 145 150 153 156 160 163 165 167 171 173 174 175 176 177 181 181 182 183 184 185 185 186 190 191 126 25 40 33 77 126 163 180 187 191 193 192 195 194 193 194 195 195 196 198 197 194 196 197 197 197 196 196 197 198 195 196 197 197 197 196 196 195 195 197 196 196 197 195 194 196 196 193 195 197 194 192 193 193 193 189 189 180 163 136 85 39 39 20 107 183 194 184 185 187 184 183 182 181 180 179 177 176 177 174 172 169 168 166 163 161 158 153 149 143 138 136 130 122 116 116 98 189 247 253 254 255 255 255 255 255 255 255\n"


// void test_readaline_basic() {
//     printf("Running test: test_readaline_basic\n");
    
//     FILE *tmp = tmpfile();  // Create a temporary file
//     assert(tmp != NULL);

//     fputs(TEST_STRING_4, tmp);
//     rewind(tmp);
//     char *line = NULL;
//     size_t len;

//     len = readaline(tmp, &line);

//     printf("len: %zu\n", len);
//     printf("strlen(TEST_STRING_4): %zu\n", strlen(TEST_STRING_4));
//     assert(len == strlen(TEST_STRING_4));   // Ensure correct length
//     assert(strcmp(line, TEST_STRING_4) == 0); // Ensure correct content
//     free(line);

//     fclose(tmp);
//     printf("test_readaline_basic passed!\n");
// }


// // void test_readaline_NULL_chars() {
// //     printf("Running test: test_readaline_NULL_chars\n");
    
// //     FILE *tmp = tmpfile();  // Create a temporary file
// //     assert(tmp != NULL);

// //     fputs(TEST_STRING_3, tmp);
// //     rewind(tmp);  // Reset file pointer to beginning

// //     char *line = NULL;
// //     size_t len;

// //     len = readaline(tmp, &line);
// //     printf("len: %zu\n", len);
// //     assert(len == 13);   // Ensure correct length
// //     assert(memcmp(line, TEST_STRING_3, 13) == 0); // Ensure correct content
// //     free(line);
  
// //     fclose(tmp);
// //     printf("test_readaline_basic passed!\n");
// // }

// void test_readaline_empty_file() {
//     printf("Running test: test_readaline_empty_file\n");

//     FILE *tmp = tmpfile();
//     assert(tmp != NULL);

//     char *line = NULL;
//     size_t len = readaline(tmp, &line);
//     assert(len == 0); // No data should be read
//     assert(line == NULL);

//     fclose(tmp);
//     printf("test_readaline_empty_file passed!\n");
// }

// void test_readaline_too_long() {
//     printf("Running test: test_readaline_too_long\n");

//     // Create a dynamically allocated 1100-character string
//     size_t long_length = 1101;  // Exceeds 1000
//     char *long_string = malloc(long_length);
//     assert(long_string != NULL);

//     memset(long_string, 'A', long_length);  // Fill with 'A'
//     long_string[long_length] = '\n';  // Add a newline

//     // Write it to a temporary file
//     FILE *tmp = tmpfile();
//     assert(tmp != NULL);
//     fputs(long_string, tmp);
//     rewind(tmp);

//     char *line = NULL;

//     // Fork to catch program exit (error handling test)
//     pid_t pid = fork();
//     if (pid == 0) {
//         readaline(tmp, &line);  // Should cause an error & exit
//         exit(1);  // If we reach here, the test failed
//     }

//     int status;
//     waitpid(pid, &status, 0);
//     assert(WIFEXITED(status));  // Ensure child process exited
//     assert(WEXITSTATUS(status) != 0);  // Ensure it exited with failure

//     free(long_string);
//     fclose(tmp);

//     printf("test_readaline_too_long passed! (Detected overly long input)\n");
// }

// void test_readaline_null_argument() {
//     printf("Running test: test_readaline_null_argument\n");

//     FILE *tmp = tmpfile();
//     assert(tmp != NULL);

//     char *line = NULL;

//     // Should exit, but we'll use assert to simulate behavior
//     if (fork() == 0) {
//         readaline(NULL, &line);
//         exit(1);  // If it doesn't exit, we fail
//     }
    
//     if (fork() == 0) {
//         readaline(tmp, NULL);
//         exit(1);
//     }

//     fclose(tmp);
//     printf("test_readaline_null_argument passed!\n");
// }

// void test_construct_injected_sequence()
// {
//     struct {
//         const char* line;
//         int expected_atom_len;
//         const char* expected_atom;
//     } test_cases[] = {
//         {"123\n", 0, ""},
//         {"hello\n", 5, "hello"},
//         {"a1b2c3\n", 3, "abc"},
//         {"x100y200z255\n", 3, "xyz"},
//         {"   42  \n", 5, "     "},
//         {"\0\0\0\n", 3, "\0\0\0"},
//         {"A.\0he123l\0oo245\0m10n\n", 12, "A.\0hel\0oo\0mn"}
//     };

//     for (size_t i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++)
//     {
//         printf("\n");
//         printf("Running Test Case %zu...\n", i + 1);

//         char* atom_val = malloc(100);
//         if (!atom_val) {
//             perror("Failed to allocate memory for atom_val");
//             exit(1);
//         }

//         int len = 
//             construct_injected_sequence(test_cases[i].line, atom_val);

//         // printf("len: %d\n", len);
//         // printf("test_cases[i].expected_atom_len: %d\n\n", test_cases[i].expected_atom_len);

//         assert(len == test_cases[i].expected_atom_len);
//         assert(memcmp(atom_val, test_cases[i].expected_atom, len) == 0);

//         // printf("Seq_length(newRow): %d\n", Seq_length(newRow));
//         // printf("test_cases[i].expected_seq_count: %d\n", test_cases[i].expected_seq_count);

//         free(atom_val);

//         printf("Test Case %zu Passed!\n", i + 1);
//         printf("\n");
//     }
// }



