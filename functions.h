#ifndef FUNCTIONS_H
#define FUNCTIONS_H


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

// #include "atom.h"
#include "seq.h"



int diff_nums_chars1(const char *line, Seq_T matrix, Seq_T atom_sequence);
int diff_nums_chars2(const char* line, const char* correct_atom, Seq_T matrix, 
                                                int width);
int separate(const char* line, Seq_T newRow, char* atom_val);
int check_atoms(Seq_T atom_sequence, const char* atom);
Seq_T correct_matrix(Seq_T matrix, int size_matrix, int index, int width);

//For testing
void printing_matrix(Seq_T matrix, int width, int heigth);
Seq_T writeRowToBinary(Seq_T seq, int width);
// void printing_atom_seq(Seq_T row);

void print_sequence(Seq_T sequence);

int construct_injected_sequence(const char* line, char* atom_val);
void construct_newRow(const char* line, Seq_T newRow);

#endif


