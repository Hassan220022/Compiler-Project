Compiler Project

A compiler is a crucial tool in software development, it is like a translator for computer programs, converting 
high-level source code written by developers into machine code that computers can understand and execute. The 
process of compilation is divided into several phases, and the first three are particularly significant.

A) General Phases 
  • Phase 1 (Lexical Analyzer): 
Convert source code into a set of tokens.
                             1. Reserved words
                             2. Special Symbols
                             3. Numbers
                             4. Identifiers
                             5. Comments
  • Context Free Gramme (CFG): 
To be able to continue the next phases, you have to design an appropriate C.F.G. The C.F.G. has to be as general 
as possible for the structure of the given statement only.

  • Phase 2 (Syntax Analyzer): 
Focusing on the code's structure and forming an abstract syntax tree.

  • Phase 3 (Semantic Analyzer): 
Check the code's meaning and context to ensure it aligns with the language's semantics. These initial phases pave 
the way for the compiler to understand the code's structure and meaning.

                              IF (X < Y) {X++} 

                                    High level language

                             Lexical Analyzer

                                     Tokenizer
                             Syntax Analyzer   +    CFG
                                     Parser
                             Semantic AnalyzerSemantic Tree

B) The Parser
 • The parser required for this project is Parse Tree.
 • This type of parser is a geometrical representation of the derived string.

C) The Statement

 • The statement to be implemented in this project is the For-loop statement.

                  For (int X = 0; X<= 100; X++) 
                  {
                     Sum+=X
                  }

D) REQUIRMENTS 
     • You must show (print) each of the inputs and outputs for each phase.
     • You must print the designed C.F.G that is fed to the required parser.
     • You must make a briefly discussing report for this project, which includes:
       1. A simple pseudo-code or flowchart of your design. You don’t have to go into the 
        details of every bit of your design.
       2. A screenshot of the inputs and outputs mentioned above.
       3. Fill out the tasks and responsibilities of each member of the team.
       4. Upload your full source code archived in a non-password protected ZIP archive 
        file. The naming of the file shall be “<Group#> - <Project#>.zip”.
        ▪ If your group# is J and the project# is 1., the file name will be J – 1.zip

