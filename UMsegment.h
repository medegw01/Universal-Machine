#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "assert.h"
#include "uthash.h"
#include "seq.h"
#include <uarray.h>

#ifndef SEGMENT
#define SEGMENT

/*
 the map_flag indicated whether a memory address is mapped if 1
 or unmaped if 0

 word_offsets store an array of words of length equal to $r[C]
 and size uint_32t
*/
typedef struct {
    uint32_t map_flag;
    Seq_T word_offsets;
} memory_segment;


/*
 this is a table that stores the memory_segment to a particular
 key/id
*/
typedef struct {
    uint32_t id;
    memory_segment *segment;
    UT_hash_handle hh;
} memoryTable_space;

typedef struct {
    memoryTable_space *memory;
    uint32_t id;
} cl;



extern void UMsegment_initialize();
extern cl *UMsegment_Map(uint32_t id_gen, Seq_T inst_seq, \
                memoryTable_space *memory_table,  Seq_T unmaped_ids);
extern void UMsegment_Unmap(uint32_t id, memoryTable_space \
                   *memory_table, Seq_T unmaped_ids);
extern uint32_t *UMsegment_load(uint32_t id, uint32_t offset,\
               memoryTable_space *memory_table);
extern void UMsegment_store(uint32_t id, uint32_t offset, \
              uint32_t val, memoryTable_space *memory_table);
extern void UMsegment_free(memoryTable_space *memory_table, Seq_T\
            maped_ids);
extern void initialize_seq();

extern uint32_t UM_end_of_program(memoryTable_space *memory_table);
extern void UMsegment_loadProgram(uint32_t id, uint32_t offsets, \
           memoryTable_space *memory_table, Seq_T unmaped_ids);
#endif
