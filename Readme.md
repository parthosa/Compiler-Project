# Compiler

This is a toy compiler written in C for course CSF363 at **BITS Pilani**

**Known Issues**: Crashes with GCC > 6 (ships with Ubuntu 16.04)

Tested with GCC 6 and below. It works perfectly for them!
The compiler follows the language given in [language/grammar.txt](language/grammar.txt) and [language/Language Specifications.pdf](language/Language%20Specifications.pdf).

## Running Instructions

```bash
$ make #compile the compiler
$ ./build/pcc testcases/test1.txt # executes the code on testfile test1.txt
```

### Testcases
Find the testcases: [testcases/](testcases/)

