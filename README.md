# Attack lab 
---
This file contains materials for one instance of the attacklab.

## About the files
* Work Files:
    These are the files you need to read in order to accomplish the lab:
    * **ctarget**
    Linux binary with code-injection vulnerability.  To be used for phases
    1-3 of the assignment.
    * **rtarget**
    Linux binary with return-oriented programming vulnerability.  To be
    used for phases 4-5 of the assignment.
    * **starget**
    Linux binary with canary protection and return-oriented programming vulnerability. 
    To be used for phase 6 of the assignment.
    * **cookie.txt**
    Text file containing 4-byte signature required for this lab instance.
    * **farm.c**
    Source code for gadget farm present in this instance of rtarget.  You
    can compile (use flag -Og) and disassemble it to look for gadgets.
    * **hex2raw**
    Utility program to generate byte sequences.  See documentation in lab
    handout.
* Answer Files:
    These are the files for MY ANSWERS, you don't need to read before you finish the lab.
    * **answers.txt**
    Input strings which you need to figure out.
    * **assembler_codes.txt**
    Attack codes in assembly languege which you need to insert in some phases.

## About basic instructions
* instruction to survive <br/>
find and click  _attacklab.pdf_ , the handout, then read it over and over again.\^_^
* untar the bag
> \> $  tar -xvf target208.tar
* examine your answers
> \> $  ./hex2raw < exploit_string.txt | ./ctarget

note: _exploit_string.txt_ contains **your** answer string, and you can apply this to _./rtarget_ and _./starget_ as well.

* get the assembler code
> \> \$ objdump -d ./*target <br/>
\> \$ objdump -d -j .rodata ./*target

note: the first instruction lets you get the executable part of the program, while the second one gives you the _.rodata_ section. You can apply this to other sections, such as _.data_ too.

* run gdb to debug (important and necessary)
> \> $ ./hex2raw < exploit_string.txt > exploit_out.txt<br/>
\> \$  gdb -q ./*target <br/>
\> \$ breakpoint [function_name] <br/>
\> \$ run < exploit_out.txt <br/>
\> \$ print \$rax <br/>
\> \$ x /20xb [address in memory]

note: these are the basic instructions to use gdb, see [this webpage](https://zhuanlan.zhihu.com/p/196763202) for more details. 

## About a trick
Want to know the answer to phase 5? Try to use the gadget _lea (%rdi,%rsi,1),%rax_.

## some word to say

_enjoy the lab and have fun_ \^_^
