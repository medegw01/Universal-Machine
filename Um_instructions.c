/**************************************************************
 * UM_instructions.c                                          *
 * COMP 40  Homework 6                                        *
 * written by: Michael Edegware & Bhushan Suwal               *
 * Date:       4/14/2017                                      *
 * Purpose:    execute an encoded instruction in a word       *
               according to the op code of the word           *
 **************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "assert.h"
#include "fmt.h"
#include "seq.h"
#include "UMsegment.h"
#define num_registers 8;

uint32_t c_mov(uint32_t ra, uint32_t rb, uint32_t rc )
{
    if (rc != 0 )
        return rb;
    else return ra;
}


uint32_t seg_load(uint32_t rb, uint32_t rc, memoryTable_space \
    *memory_table )
{
    return *UMsegment_load(rb, rc, memory_table);
}


void seg_store(uint32_t ra, uint32_t rb, uint32_t rc, \
      memoryTable_space *memory_table)
{
    UMsegment_store(ra, rb, rc, memory_table);
}

uint32_t add(uint32_t rb , uint32_t rc)
{
    return rb + rc;
}

uint32_t multiply(uint32_t rb, uint32_t rc)
{
    return rb * rc;
}

uint32_t divide(  uint32_t rb, uint32_t rc)
{
    assert(rc != 0);
    return rb / rc;
}

uint32_t bitwise_nand( uint32_t rb, uint32_t rc)
{
    return ~( rb & rc );
}

void halt(memoryTable_space *memory_table, Seq_T unmaped_ids, \
        cl *tmp)
{
    UMsegment_free(memory_table, unmaped_ids);
    free(tmp);
    exit(0);
}

cl *map_segment(uint32_t rc, uint32_t id, memoryTable_space \
     *memory_table,  Seq_T unmaped_ids)
{
    Seq_T new_seq = Seq_new(rc);
    new_seq = malloc( sizeof(new_seq));
    for (unsigned i = 0; i < rc; i++) {
        Seq_addhi(new_seq, 0);
    }
    return UMsegment_Map(id, new_seq, memory_table, unmaped_ids);
}

void unmap_segment(uint32_t rc, memoryTable_space *memory_table, \
     Seq_T unmaped_ids)
{
    UMsegment_Unmap(rc, memory_table,unmaped_ids);
}

void output(uint32_t rc)
{
    assert(rc <= 255);
    printf("%c",(char) rc);
}

uint32_t input()
{
    char c = getc(stdin);
    if (c == EOF) {
        int32_t input = -1;
        return input;
    }
    else {
        return c;
    }
}

void load_program(uint32_t rb, uint32_t rc,memoryTable_space \
     *memory_table, Seq_T unmaped_ids)
{
    UMsegment_loadProgram(rb, rc, memory_table, unmaped_ids);
}

/*Very simple function, but kept in the interface because this is clearer
to the user */
uint32_t load_val(uint32_t val)
{
    return val;
}

