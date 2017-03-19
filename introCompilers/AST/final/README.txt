==========
Developers
==========
Mohammad Alaul Haque Monil
Soren Rasmussen


=======================================
           STEPS TO TEST
=======================================

Step 1
======
To compile all required files enter the following command. All the required
files are copied and compiled.

shell$ make


Step 2
======
To test [test.file] using the following command

shell$ ./quack path/to/file/[test.file]


=======
DETAILS
=======
The quack compiler is the binary named "quackCompiler". "quack" is a bash script
that internally calls "quackCompiler" on [test.qk], compiles the generated [test.c], and runs
the resulting [test] binary.

