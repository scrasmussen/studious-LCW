C=gcc
CC=g++
FILES=quack.l quack.y Node.cpp Generate.cpp Builtins.c \
	      ast.cpp Node.h   Generate.h   Builtins.h
CFLAGS=-std=c++11 -g

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

all: copy node generate QC

copy:
	for file in $(FILES) ; do \
		cp ../$$file ./ ; done

flex : bison
	flex quack.l #-d

bison : quack.y 
	bison -d -t -k -v $< #-g

node Node.o:
	$(CC) -c Node.cpp  $(CFLAGS)

generate Generate.o:
	$(CC) -c Generate.cpp  $(CFLAGS)

builtins Builtins.o:
	$(C) -c Builtins.c -g

objs ast.o: node flex
	$(CC) -c ast.cpp quack.tab.c lex.yy.c $(CFLAGS)

QC: Builtins.o Node.o Generate.o ast.o quack.tab.o lex.yy.o 
	$(CC) -o quackCompiler ast.o quack.tab.o lex.yy.o Node.o Generate.o  -lfl $(CFLAGS)

gen: generate
	$(CC) -o quack ast.o quack.tab.o lex.yy.o Builtins.o Node.o Generate.o  -lfl $(CFLAGS)	


comp:
	gcc output.c Builtins.o

clean:
	rm -f *~ quackCompiler *.yy.* *.tab.* *.dot *.o *.c quack.output output.c $(FILES) \
	robot schroedinger Sqr22 Pt a.out
