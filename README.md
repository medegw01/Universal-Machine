# Universal-Machine

#### Author: 
Michael Edegware and Bhushan Suwal

#### Acknowledgements:
Than you to the TAs and Prof MArk Sheldon for
their help on Piazza:smile:

#### Architecture of the design:
Our program has three main modules:
###### 1. read_from_file
  This module, when passed a file pointer, opens a file stream 
  and takes in 32 bit words at a time. It packs each word into
  a sequence, which we are going touse as our m[0] memory segment.

###### 2. Um_instructions
  This module executes the 14 instructions of the UM. Many of these 
  instructions deal with changing words in the memory, so this module 
  interacts with the UMsegment module.

###### 3. UMSegment
  This module performs the full functionality of the memory. It maps
  and umaps, load and store words and modify the memory.
  The invariants include:
  - the actual program being excuted must be in $m[0];
  - program counter must point to a valid word;
  - atleast one segement must be mapped
  - no new segment is created is there's an umapped but allocated memory.
  - the client must keep track of id.
  - the client must allocate memory for items stored in the memory.

#### How these modules come together:
   The program first loads the memory with all of the instructions in 
   the UM program. This loading is done by the read_from_file segment,
   which returns a sequence of 32 bit words. This sequence is then 
   initialized as the m[0] segment in the memory by the UMsegment_Map()
   function. Now that we have our sequences, we enter a while loop of
   fetch-decode-execute cycles where a new  word is loaded, sent to be
   decoded and then subsequently executed.

  The main program makes use of the UMsegment module to initialize the
  m[0] memory segment and then to continuously load instructions. On ce
  those  instructions are loaded, the main decodes them using Bitpack, 
  and then uses the Um_instructions module to execute the instructions.
  Instructions like add nad multiply do the computation and return the
  correct value to the registers in main(), but some register like 
  load_program and map_segment have to manipulate memory and hence use
  the UMmemory module. 

We spent about 20hours analyzing the assignmet:expresionless:
We spent about 2 days preparing the design documents:tired_face:
We spent about 48 hours solving the problems after the analysis:cry:
