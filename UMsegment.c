/**************************************************************
 * UMsegment.c                                                *
 * COMP 40  Homework 6                                        *
 * written by: Michael Edegware & Bhushan Suwal               *
 * Date:       4/14/2017                                      *
 * Purpose:    perform all required  memory activities        *
 **************************************************************/

#include "UMsegment.h"
/* helper functions declarations */
void failure();
uint32_t *UMsegment_loadhelper(uint32_t id, uint32_t \
                 offset,  memoryTable_space *memory_table);





/*
this maps a memory segment with the words stored in a
sequence passed in and the id of memeory segment
*/
cl *UMsegment_Map(uint32_t id_gen, Seq_T inst_seq, memoryTable_space\
                                *memory_table,  Seq_T unmaped_ids)
{
    memoryTable_space *tmp;
    uint32_t old_id;
    cl *cl_tmp = malloc(sizeof(*cl_tmp));
    /*
    first check if there's mapped empty segment
           if not, use an unmapped space
    */
    if(Seq_length(unmaped_ids)!= 0) {
        old_id = *( (uint32_t*) Seq_remhi(unmaped_ids) );
        HASH_FIND_INT(memory_table, &old_id, tmp);
        tmp->id = old_id;
        Seq_free(&tmp->segment->word_offsets);
        tmp->segment->word_offsets = inst_seq;
        tmp->segment->map_flag = 1;
    }

    else {
        /*allocate memory for staructure*/
        memory_segment *a_segment = malloc(sizeof(*a_segment));
        /*populate structure*/
        a_segment->map_flag = 1;
        a_segment->word_offsets = inst_seq;
        /*add structure to table*/
        tmp =  malloc(sizeof(*tmp));
        tmp->id = id_gen;
        tmp->segment = a_segment;
        HASH_ADD_INT(memory_table, id,tmp);
        old_id = id_gen;
        id_gen++;
    }
    cl_tmp->memory = memory_table;
    cl_tmp->id = old_id;
    return cl_tmp;
}


/*
this unmaps a memory segment
*/

void UMsegment_Unmap(uint32_t id, memoryTable_space *memory_table,\
                Seq_T unmaped_ids)
{
    memoryTable_space *tmp;
    HASH_FIND_INT(memory_table, &id,tmp);
    /*check is a field outside the memory is requested; the memory
    has already been umapped; the memory $m[0]
    if about to be freed; if any condition is met, it's a runtime
        error
    */
    if(tmp == NULL || tmp->segment->map_flag == 0) {
        //failure();
        printf("already freed");
        exit(0);
    }

    else {
        tmp->segment->map_flag = 0;
        int *i = malloc(sizeof(i));
        *i = id;
        Seq_addlo(unmaped_ids, i);
      
    }
}

/*
frees the whole allocated memory from the computer memory
used to run the machine
*/
void UMsegment_free(memoryTable_space *memory_table, Seq_T\
    unmaped_ids)
{
    Seq_free(&unmaped_ids);
    memoryTable_space *tmp, *current_user;
    /* iterates through the table and free */
    HASH_ITER(hh, memory_table, current_user, tmp) {
        for (int i = 0; i < Seq_length(current_user->segment->\
              word_offsets); i++) {
            free(Seq_get(current_user->segment->word_offsets, i));
        }
        Seq_free(&current_user->segment->word_offsets);
        free(current_user->segment);
        HASH_DEL(memory_table, current_user);
        free(current_user);
    }

}


/*
gets the current line the program counter is pointing to
*/
uint32_t *UMsegment_load(uint32_t id, uint32_t offset, \
       memoryTable_space *memory_table)
{
    return UMsegment_loadhelper(id,offset, memory_table);
}


/*
it takes a value and store it in the memory
 */
void UMsegment_store(uint32_t id, uint32_t offset, uint32_t val,\
          memoryTable_space *memory_table)
{
    memoryTable_space *tmp;
    HASH_FIND_INT(memory_table, &id, tmp);
    if (tmp == NULL) {
        //failure();
        printf("cannot store values here");
        exit(0);
    }
    else {
        if (tmp->segment->map_flag == 0) {
            //failure();
            printf("space has been unmapped and value cannot be found");
            exit(0);
        }
        else {
            Seq_put(tmp->segment->word_offsets, \
                    offset, &val);

        }
    }
}


/* abort by Checked Runtime Error*/
void failure()
{
    /* abort by Checked Runtime Error*/
    printf("CRE: There has been an error.\n");
    exit(1);
}


/*
it taked the id and offset, then fetches a data from a memory
to save to the register.
it's a runtime error to load a memory space which has not been
allocated or has been freed is loaded
it also set th e program counter to the current instruction
being excuted
*/
uint32_t *UMsegment_loadhelper(uint32_t id, uint32_t offset, \
         memoryTable_space *memory_table)
{
    memoryTable_space *tmp;
    HASH_FIND_INT(memory_table, &id, tmp);
    /*first check is id has never been allocated in memory*/
    if(tmp == NULL) {
        //failure();
        printf("cannot load, id not found");
        exit(1);
    }
    /*if allocated, check if it has been freed and load if not*/
    else {
        if(tmp->segment->map_flag == 0) {
            //failure();
            printf("mapped but id is already freed");
            exit(0);
        }



    }
    return Seq_get(tmp->segment->word_offsets,\
                   offset);
}

uint32_t UM_end_of_program(memoryTable_space *memory_table)
{
    uint32_t id = 0;
    memoryTable_space *tmp;
    HASH_FIND_INT(memory_table, &id, tmp);
    return Seq_length(tmp->segment->word_offsets);
}

void UMsegment_loadProgram(uint32_t id, uint32_t offsets, \
 memoryTable_space *memory_table, Seq_T unmaped_ids)
{
    memoryTable_space *tmp;
    (void) offsets;
    HASH_FIND_INT(memory_table, &id, tmp);
    assert(tmp);
    /*unmap segment zero*/
    UMsegment_Unmap(0, memory_table, unmaped_ids);
    UMsegment_Map(0, tmp->segment->word_offsets, memory_table,\
       unmaped_ids);
    //UMsegment_load(0, offsets, memory_table); 
}
