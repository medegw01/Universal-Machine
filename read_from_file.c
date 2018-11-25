/**************************************************************
 * read_from_file.c                                           *
 * COMP 40  Homework 6                                        *
 * written by: Michael Edegware & Bhushan Suwal               *
 * Date:       4/14/2017                                      *
 * Purpose:    reads a .um file and pack it into a the memory *
 **************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "assert.h"
#include "seq.h"
#include "bitpack.h"
#include "fmt.h"
#include "uarray.h"
#include "read_from_file.h"

#define byte_width 8
#define num_bits_in_char 8

Seq_T read_file(char *filename)
{
    FILE *fp;
    fp = fopen(filename, "r");
    assert(fp != NULL);
    Seq_T word_seq = Seq_new(0);

    int current;
    while(1) {
        uint32_t *word = malloc(sizeof(word));
        uint64_t tmp = 0;
        for (int j = 3; j >= 0; j--) {
            current = getc(fp);
            if (current == EOF)
                break;

            unsigned lsb = num_bits_in_char * j;
            tmp = Bitpack_newu(tmp, byte_width, lsb, (uint64_t)current);
        }
        if (current == EOF) {
            free(word);
            break;
        }
        *word = (uint32_t) tmp;
        store_word_in_seq( word, word_seq);
    }

    fclose(fp);

    return word_seq;
}

void store_word_in_seq( uint32_t *word, Seq_T word_seq)
{
    Seq_addhi( word_seq, word);
}


