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
the resulting [test] binary. If the quackCompiler finds any errors then no C code will be
generated.

The lexer, parser, and type checker have all been fully implemented. We have been able to
successfully define the Quack methods in C and call them. Our good code presents this
using PRINT statements. Inheratence is also working and is demonstrated for the variables.
We have implemented if, elif, else and while loops. We have completed most but not all of the 
implementations of the basic methods (LESS,MORE,...) in the base classes. There are no doubt
corner cases where our C generated code would break that we haven't tested for.


===========
TypeChecker
===========
Our typechecker works for all the provided sample programs. However, we have given three bad
files to demonstrate main features of the type checker.

We have done the following checkings for the Typechecker :
* classes cannot be redefined.  Methods cannot be redefined in the same class. 
* Covariance and Contravariance for function overrriding. 
* If the superclass has a field x of type c, then the subclass must initialize field x to a value
  which is some subclass of c.
* Each method must return the type it is declared to return, on all execution paths.
* Each variable must be initialized before it is used, on all syntactically possible execution paths
* Each method call is well-typed with respect to the static type of the receiver object.
