# CS-40-HW1
Darius-Stefan Iavorschi (diavor01)
Alijah Jackson (ajacks11)
->

filesofpix Assignment

**Problem Statement:** 
  Extracting image data from the corrupted files by identifying and separating the legitimate rows to reconstruct the uncurrpted image

**Use Cases:** 
-  Restore a corrupted PGM file by identifying valid image rows and discarding injected ones.
-  Validate input files for structural integrity.
-  efficiently read files containing arbitrary-length lines

**Assumptions:**
-  Input:
  -    Injected rows and original rows can be differentiated by their infusion sequence of non-digit bytes.
  -    Images are atleast a 2x2 grid
  -    The input can come from both stdin or from a file
-  Corruption
  -    The corrupted images originate from "plain" PGM files(P2)
  -    Each original row is terminated by a newline character (\n) that remains unchanged.
  -    Corruption does not alter the row order but adds extra rows
  
**Constraints:**
-  The restoration program should process even large images efficiently under 20 seconds
-  Hanson’s data structures are available and must be used when appropriate(except for arrays)

**Hanson's data abstractions used and their purposes:**
-  Sequences:
   -  they can store data sequentially
   -  they can resize dynamically
-  Atoms
   -  they allow for atom equality, which is essential for comparing the injected sequences
   
**File Descriptions:**
-  main.c: checks the type of input received and passes it on to the restoration function
-  readaline.c and its header file: reads a corrupted line of text at a time. A line can contain any ASCII characters (including NULL) and is guaranteed to end with "\n"
-  functions.c and its header file: represent helper functions for the restoration function
-  restoration.c and its header file: the file responsible for the restoration process from a corrupted plain file to a p5 pgm file
-  conversion.c and its header file: helper file that includes functions to convert the sequence of numbers to binary and write both the contents and header to a p5 format

**Architecture:**
-  For readaline, we will be using a pointer to char called buffer to store the characters. Buffer stops receiving input when finding the endline character.
-  For the restoration part, we need 2 data structures: a matrix in numbers matrix_nums (a pointer to pointer to integer), which will represent the final, restored p2 pgm file, and a pointer to char, which will store Hanson's atoms. We prefer this implementation over a pointer to a pointer to char because Atoms allow pointer equality. Since the lines from the original file have been injected with the same sequence of characters, all atoms we store must be equal.
-  We will use the readaline function to read the corrupted file line by line. The input will be separated into 2 parts: matrix[i] (a pointer to int, where we store the numbers from the input line) and atoms[i] (where we store anything else). After every line iteration, we check if we found any 2 equal atoms (which should be easy considering atoms allow pointer equality). We repeat the process until we find 2 equal atoms, atoms[x] and atoms[y]. We delete all the lines from both data structures besides x and y.
-  Now that we know the correct sequence of characters with which the original lines have been injected, we simply need to check every corrupted line individually and store the numbers in matrix_nums if the sequence matches.
-  Once we have the correct sequence of lines along with the dimensions of the file, this sequence is passed to the conversion process and output to stdout as the PGM header with the magic number, dimensions, and max grayscale value followed by the content of the file expressed by single bites as opposed to individual character in the P2 format.

**Data structures used:**
-  Seq_T atom_sequence: Represents a sequence of atoms. The first step is establishing the correct sequence of characters the original lines have been injected with. We add a new atom to the sequence until we find identical atoms.
-  Seq_T newRow: eepresenting a sequence of numbers
-  Seq_T matrix: representing a sequence of newRows

**Implementations:**
-  readaline:
    -  The readaline function reads characters one by one from the input file and stores them in a buffer (the buffer is allocated dynamically so that its size can change). The loop stops if it encounters the endline character or the end of the file. Then *datapp gets updated and the size of the buffer is returned. If the size is 0 (meaning we no characters have been read, and thus we reached the end of the line), the buffer is freed and the function returns 0
-  restoration
    -   this is the main function for handling of both decrypting and outputing the uncorruted pgm images. In this file we use readaline to process a corrupted file line by line, storing numbers in matrix_nums and characters in atoms, leveraging pointer equality to identify duplicate sequences. Once duplicates are found, we restore the original PGM file by filtering lines with the correct sequence, converting the data, and outputting it in PGM format with the appropriate header and binary content. 
-  WriteToBinaryt
    -  the writeToBinary function reads the characters from the sequence of characters and converts them to bytes that can outputted. This method is called multiple times over a conversion as it adds it to the correct matrix of btyes
  
**Tests:**
  
   - The readaline function will be tested using diff for each corrupted file provided. Additional files will be provided to test each expected error. After that, we will try using larger pgm files (width>1000).
     
   - Diff can be used on original p5 formatted files to compare the converted uncorrupted image vs the uncorrupted image that went through no processing. If no results show that means the files are identical

   - We test reading lines from a file or standard input by providing various inputs (standard strings, empty files, long lines, and null arguments) for the purpose of ensuring the readaline function correctly reads lines, handles edge cases, and reports errors appropriately.

  -  We test atom handling by providing sequences of atoms and lines with mixed numbers and characters for the purpose of ensuring the program correctly identifies existing atoms, extracts injected sequences, and handles non-existing atoms.

  -  We test number and character separation by providing lines with mixed numbers, only numbers, only characters, and empty lines for the purpose of ensuring the program correctly separates numbers and characters and stores them in appropriate data structures.

  - We test matrix construction by providing lines with mixed numbers and characters, as well as invalid data (e.g., negative numbers, numbers exceeding the max grayscale value) for the purpose of ensuring the program correctly constructs rows of numbers and handles edge cases gracefully.

  - We test file restoration by providing corrupted PGM files, large files, and edge cases (empty files, files with only headers, and files with invalid data) for the purpose of ensuring the program correctly restores the files, handles large inputs efficiently, and produces appropriate error messages or output.

  - We test file restoration accuracy by comparing restored files with original uncorrupted files using diff for the purpose of ensuring the restored files are identical to the originals and the program works as expected.
