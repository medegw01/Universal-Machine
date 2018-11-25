/**************************************************************
 * UM_main.c                                                  *
 * COMP 40  Homework 6                                        *
 * written by: Michael Edegware & Bhushan Suwal               *
 * Date:       4/14/2017                                      *
 * Purpose:   co-ordinated the um machines as to how an       *
 *            instruction should be managed                   *
 **************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "assert.h"
#include "seq.h"
#include "bitpack.h"
#include "Um_instructions.h"
#include "UMsegment.h"
#include "read_from_file.h"
#include "fmt.h"


#define opcode_lsb 28
#define opcode_width 4
#define regC_lsb 0
#define regB_lsb 3
#define regA_lsb 6
#define register_width 3
#define val_width 25
#define val_lsb 0
#define regA13_lsb 25

int32_t decodeAndExecute(uint32_t r[], uint32_t word, int32_t counter,\
      uint32_t id_gen, memoryTable_space *memory_table,  Seq_T unmaped_ids,\
       cl *tmp);

int main(int argc, char* argv[])
{
    (void) argc;

    Seq_T instructions;
    Seq_T unmaped_ids;
    uint32_t *program_counter;
    memoryTable_space *memory_table = NULL;
    uint32_t id_gen;

    unmaped_ids = Seq_new(0);
    id_gen = 0;
    instructions= read_file( argv[1] );


    cl *tmp;
    tmp = UMsegment_Map(id_gen,instructions, memory_table, unmaped_ids);
    memory_table = tmp->memory;
    id_gen++;
    program_counter = UMsegment_load(0,0, memory_table);

    uint32_t registers[8] = {0};




    uint32_t counter = 0;
    while(counter  < UM_end_of_program(memory_table)) {
        program_counter = UMsegment_load(0, counter, memory_table);
        decodeAndExecute(registers, *program_counter, counter, id_gen,\
                      memory_table, unmaped_ids,tmp);
        counter++;
    }
    return 0;
}

int32_t decodeAndExecute(uint32_t r[], uint32_t word, int32_t counter, \
   uint32_t id_gen, memoryTable_space *memory_table,  Seq_T unmaped_ids,\
     cl *tmp)
{
    word = (uint64_t) word;
    uint64_t opcode = Bitpack_getu(word, opcode_width, opcode_lsb);

    if (opcode < 13) {
        uint64_t ra = Bitpack_getu(word, register_width, regA_lsb);
        uint64_t rb = Bitpack_getu(word, register_width, regB_lsb);
        uint64_t rc = Bitpack_getu(word, register_width, regC_lsb);

        switch (opcode) {
        case 0:
            r[ra] = c_mov( r[ra], r[rb], r[rc] );
            break;
        case 1:
            r[ra] = seg_load( r[rb], r[rc],memory_table );
            break;
        case 2:
            seg_store( r[ra], r[rb], r[rc], memory_table);
            break;
        case 3:
            r[ra] = add( r[rb], r[rc] );
            break;
        case 4:
            r[ra] = multiply( r[rb], r[rc] );
            break;
        case 5:
            r[ra] = divide( r[rb], r[rc] );
            break;
        case 6:
            r[ra] = bitwise_nand( r[rb], r[rc] );
            break;
        case 7:
            halt(memory_table,unmaped_ids, tmp);
            break;
        case 8:
            tmp = map_segment(r[rc], id_gen+1, memory_table, unmaped_ids);
            r[rb] = tmp->id;
            break;
        case 9:
            unmap_segment( r[rc],  memory_table, unmaped_ids );
            break;
        case 10:
            output(r[rc]);
            break;
        case 11:
            r[rc] = input();
            break;
        case 12:
            load_program( r[rb], r[rc], memory_table, unmaped_ids);
            break;
        }
    }
    else if (opcode == 13) {
        uint32_t val = Bitpack_getu(word, val_width, val_lsb);
        uint32_t reg_location = Bitpack_getu(word, register_width, \
            regA13_lsb);
        r[reg_location] = load_val(val);
    }
    return counter;
}
