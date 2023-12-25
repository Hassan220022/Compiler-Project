# SIC Assembler

The `assembler_sic.c` file contains the implementation of an assembler for the Simplified Instructional Computer (SIC). The assembler reads assembly language source code and produces the corresponding machine code.

## How it works

The assembler works in two passes:

1. **First Pass**: The first pass reads the source code and builds the symbol table. It scans the source code line by line, identifies labels (symbols), and stores them in the symbol table with their corresponding addresses.

2. **Second Pass**: The second pass generates the machine code using the symbol table. It translates each instruction to its binary form. For instructions with a symbol as an operand, it looks up the symbol in the symbol table to get its address.

The `addr` function is used in the process of converting addresses to their hexadecimal form. It takes a character representing a digit ('0' to '7') and converts it to a hexadecimal character ('8' to 'F').

## How to run

To compile the program, use the following command:

```make
make run
```

to clean the out put only run 

```make
make clean
```

or if you want to run without make just run in terminal 

```bash
gcc assmebler_sic.c -o sic
./sic
```