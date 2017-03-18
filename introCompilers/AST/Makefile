C=gcc
CC=g++
TESTDIR=/home/faculty/michal/classes/cis461/17W/trial
TD=trial/samples
LEXFILE=$(TD)/LexChallenge.qk
QFILE=$(TD)/hands.qk
QFILE=$(TD)/LexChallenge.qk
QFILE=$(TD)/robot.qk
QFILE=robot.qk
QFILE=$(TD)/Sqr.qk
QFILE=$(TD)/input.qk
QFILE=Sqr.qk
QFILE=Sqr22.qk
# QFILE=SqrClass.qk
# QFILE=bad_init.qk



CFLAGS=-std=c++11 -g

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

all: node generate ast gen

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

ast: Builtins.o Node.o Generate.o ast.o quack.tab.o lex.yy.o 
	$(CC) -o quack ast.o quack.tab.o lex.yy.o Node.o Generate.o  -lfl $(CFLAGS)

gen: generate
	$(CC) -o quack ast.o quack.tab.o lex.yy.o Builtins.o Node.o Generate.o  -lfl $(CFLAGS)	
	./quack ${QFILE}
	@echo
	cat output.c

test: gen
	gcc output.c Builtins.o
	@echo "===Running Binary==="
	a.out

comp:
	gcc output.c Builtins.o

clean:
	rm -f *~ quack *.yy.* *.tab.* *.dot *.o output.c
