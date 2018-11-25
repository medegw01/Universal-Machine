
#include "seq.h"
#include "UMsegment.h"

/* function declarations*/
uint32_t c_mov(uint32_t ra, uint32_t rb, uint32_t rc );
uint32_t seg_load(uint32_t rb, uint32_t rc, memoryTable_space\
    *memory_table);
void seg_store(uint32_t ra, uint32_t rb, uint32_t rc, \
     memoryTable_space *memory_table);
uint32_t add(uint32_t rb , uint32_t rc);
uint32_t multiply(uint32_t rb, uint32_t rc);
uint32_t divide(  uint32_t rb, uint32_t rc);
uint32_t bitwise_nand( uint32_t rb, uint32_t rc);
void halt(memoryTable_space *memory_table, Seq_T unmaped_ids,\
   cl *tmp);
cl *map_segment(uint32_t rc, uint32_t id, memoryTable_space \
   *memory_table,  Seq_T unmaped_ids);
void unmap_segment(uint32_t rc, memoryTable_space *memory_table,\
    Seq_T unmaped_ids);
void output(uint32_t rc);
uint32_t input();
void load_program(uint32_t rb, uint32_t rc,memoryTable_space\
  *memory_table, Seq_T unmaped_ids);
uint32_t load_val(uint32_t val);
