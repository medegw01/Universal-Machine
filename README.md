# Universal-Machine
#### Purpose and overview
The purpose of this assignment is to understand virtual-machine code (and by extension machine code) by writing a software implementation of a simple virtual machine.

#### Specification of the Universal Machine
##### Machine State
The UM has these components:
 - Eight general-purpose registers holding one word each
 - An ever-changing collection of arrays of words, each referred to by a distinct 32-bit identifier
 - A 1x1 character resolution console capable of displaying ASCII characters and performing input and output of unsigned 8-bit characters
 - A 32-bit program counter
 
 One distinguished array is referred to by the 32-bit identifier 0 and stores the *program*. This array is called the '0' array.
#### Notation
To describe the locations on the machine, we use the following notation:
- Registers are designated r[0] through r[7]
- The array identified by the 32-bit number i is designated a[i]. The '0' array is designated a[0].
- A word at offset n within array i is designated element(a[i], n).
#### Initial state
The UM is initialized by providing it with a program, which is a sequence of 32-bit words. Initially
- The '0' array a[0] contains the words of the program.
- An array may be active or inactive. Initially, a[0] is active and all other arrays are inactive.
- All registers are zero.
- The program counter points element(a[0], 0), i.e., the first word in the '0' array.
#### Execution cycle
At each time step, an instruction is retrieved from the word pointed to by the program counter. The program counter is advanced to the next word, if any, and the instruction is then executed.

#### Instructions' coding and semantics
The Universal Machine recognizes 14 instructions. The instruction is coded by the four most significant bits of the instruction word. These bits are called the *opcode*.
Some instructions ignore one or more of the register numbers A, B, and C.|width 0pt depth 6pt 

| *Number	Operator8* | Action |
| ----------------------- | --------- |
|  Conditional Move |	if r[C] !=0 then r[A] :=r[B] |
 |  Array Index|	r[A] :=element(a[r[B]], r[C])|
| Array Update	| element(a[r[A]], r[B]) :=r[C]|
 |	Addition|	r[A] :=(r[B] + r[C]) $$mod\;2^{32}$$|
 |	Multiplication	|r[A] :=(r[B] ×r[C]) mod2^32|
 |	Division |	r[A] :=[r[B]  / r[C]]|
|	Bitwise NAND|	r[A] :=~(r[B]  & r[C]|
 |	Halt|	Computation stops|
| Activate Array	|A new array is created with a number of words equal to the value in r[C]. Each word in the new array is initialized to 0. A bit pattern that is not all zeroes and that does not identify any currently active array is placed in r[B]. Array a[r[B]] becomes active|
|	Inactivate Array |	The array a[r[C]] becomes inactive. Future allocations may reuse the identifier r[C].|
|	Output |	The value in r[C] is displayed on the console immediately. Only values from 0 to 255 are allowed.|
|	Input|	The universal machine waits for input on the console. When input arrives, r[C] is loaded with the input, which must be a value from 0 to 255. If the end of input has been signaled, then r[C] is loaded with a word in which every bit is 1|
|	Load Program|	Array a[r[B]] is duplicated, and the duplicate replaces a[0], which is abandoned. The program counter is set to point to element(a[0], r[C]). If r[B] = 0, the load-program operation is expected to be extremely quick|
|	Load Value	|See semantics for "other instruction" in Section |

#### Semantics of UM instructions [*]

##### Three-register instructions
Most instructions operate on three registers. The registers are identified by number; we'll call the numbers A, B, and C. Each number coded as a three-bit unsigned integer embedded in the instruction word. The register C is coded by the three least significant bits, the register B by the three next more significant than those, and the register A by the three next more significant than those:

                                      A     C
                                      |     |
                                      vvv   vvv                    
              .--------------------------------.
              |VUTSRQPONMLKJIHGFEDCBA9876543210|
              `--------------------------------'
               ^^^^                      ^^^
               |                         |
               opcode                    B
Semantics are given in Figure [<-].
##### One other instruction
[*]
One special intruction, with opcode 13, does not describe registers in the same way as the others Instead, the three bits immediately less significant than opcode describe a single register A. The remaining 25 bits indicate a value, which is loaded into r[A].

                   A  
                   |  
                   vvv
              .--------------------------------.
              |VUTSRQPONMLKJIHGFEDCBA9876543210|
              `--------------------------------'
               ^^^^   ^^^^^^^^^^^^^^^^^^^^^^^^^
               |      |
               |      value
               |
               opcode == 13
##### Failure modes
The behavior of the Universal Machine is not fully defined; under circumstances detailed below (and only these circumstances), the machine may fail.

If at the beginning of a machine cycle the program counter points outside the bounds of a[0], the machine may fail.
If at the beginning of a cycle, the word pointed to by the program counter does not code for a valid instruction, the machine may fail.
Array index or array update of an inactive array may fail.
Array index or array update of a word outside the bounds of an array may fail.
If an instruction inactivates a[0], or if it inactivates an array that is not active, the machine may fail.
If an instruction divides by zero, the machine may Fail.
If an instruction loads a program from an array that is not active, then the machine may fail.
If an instruction outputs a value larger than 255, the machine may fail.
In the interests of performance, failure may be treated as an unchecked run-time error. Even a core dump is OK.
## Implementation:
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
#### Analysis:
###### Time Needed to Execute 50 Million Instructions:
Our UM took 8.52 seconds of CPU time and 0.00 seconds of system time, 
for a total of 8.52 seconds, to run midmark.um.  
After running callgrind, we know that there are in total 37597556410 instructions pertaining to midmark.um.
It follows that our UM program will need to spend
```
    (CPU_time / num_instructions) * 50,000,000
        = 8.52 / 85070522 * 50,000,000 
        = 5.00s
``` 
approxiamtely 5 seconds to run 50 million instructions. 

###### List of Unit Tests:
- halt.um
        Uses halt instruction.
- halt-verbose.um
        Uses halt instruction. Tests that running halt 
        actually stops computation by adding extra instructions after
        halt.
- add.um add.1
        Adds two register values (50 and 6) together, outputs the ascii
        number of 56 which is "8", and halts.
- cmov.um cmov.1
        Uses condition move instruction. First, tests that when 
        rC != 0, rA wouldnt be set equal to rB. Outputs rA to see if its value 
        doesnt change. Then, tests when rC = 0, outputs rA and rB to see if 
        they have the same value. Finally, halts the program.
- mul.um mul.1
        Multiplies two register values (8 and 9) together, outputs the 
        ascii number of 72 which is "H", and halts.
- mod32.um mod32.1
        Since both add and multiply instructions include a (mod 2^32) 
        at the end. So result that are bigger than 2^32 will be modified to its
        remainder. mod32.um checks whether the remainders are right when the 
        results are bigger than 2^32.
- div.um div.1
        Uses div instruction. First, test the result of 1089 / 33, which 
        is divisble, and outputs the result. Next, test the result of 121 / 1, 
        whcich should be 121 itself. Finally, test something that is not 
        divisible, 121/2, and the output result should be truncated to an 
        integer of 60.
- nand.um nand.1
        Tests that nand works for two nonzero values by loading two 
        values that nand to a printable value, and prints it.
- input-add.um input-add.0 input-add.um
        Tests input instruction with the help of add instruction.
        Adds the two inputs together and print out the result to see if its 
        doing the addition right.
- map-unmap.um 
        Tests map and unmap instructions. Tests special cases like
        mapping 0 segment, 1 segment, and a general case of of 10 segments.
- load-store.um load-store.1
        Tests segment load and segment store instructions. First 
        maps a segment that contains only one word. Next, loads the word from 
        the newly mapped segment (it should be zero), and adds it with 48. If 
        the newly initialzied word indeed has a value of 0, the sum of it and 48
        should output 48. Then store the value 48 back to the word in the newly
        mapped segment, and then add itself together to get 96. Finally outputs 
        the result to see if it is 96 in ascii.
- load-seg0.um
        Tests segment load and segment store instructions. First, 
        load the instruction in $m[0][6] (which is an add instruction) into a 
        register X. And then stores that instruction into $m[0][8]. The original
        instruciton is $m[0][8] is a multiply instruction, now it is replaced
        by the add instruction. Tests to see the output of the multiplication
        is skipped. So the result of the addition is printed twice.
- lp-jump.um lp-jump.1
        Tests load program instruction to see if it jumps according 
        to the instruction. Loads segment 0 itself to segment and set the 
        program counter to $m[0][6]. Tests to see if the program jumps through
        all the instructions before the 6th instruction. Since instructions 
        before $m[0][6] contains an output instruction. If nothing is printed 
        out, then it jumps successfully.
- lp.um lp.1
        Tests load program instruction. First, maps a segment of size 5, 
        and stores its id in r3. Next moves 5 instructions 1 by 1 to $m[r3]. 
        Then, calls load program, and set counter to 0. Tests to see if 
        newly-stored instructions in $m[r3] are performed. It should output a 
        sum from the addtion of 13 + 20, which is 33.
In addition, we have a set of unit tests for our memory segment module 
MemSeg. This test can be run as a separate executable:
>. /memseg-test

It contains unit tests for each memory segment function.
#### Run:
All tests (including the memseg-test) can be run as an executable:
> ./um-test
Which utilizes popen() and system() function calls to execute each command
with valgrind.

We spent about 20hours analyzing the assignmet:expressionless: .
We spent about 2 days preparing the design documents:tired_face: .
We spent about 48 hours solving the problems after the analysis:cry: .
