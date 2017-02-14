CC=g++
TESTDIR=/home/faculty/michal/classes/cis461/17W/trial
TD=trial/samples
QFILE=$(TD)/hands.qk
QFILE=$(TD)/LexChallenge.qk
QFILE=$(TD)/robot.qk
#QFILE=$(TD)/Sqr.qk
QFILE=$(TD)/input.qk
#QFILE=Sqr.qk
LEXFILE=$(TD)/LexChallenge.qk
# QFILE=$(TD)/bad_init.qk

CFLAGS=-std=c++11

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

all: node ast run

flex : bison
	flex quack.l #-d

bison : quack.y 
	bison -d -t -k -v $< #-g

node Node.o:
	$(CC) -c Node.cpp  $(CFLAGS)

objs ast.o: node flex
	$(CC) -c ast.cpp quack.tab.c lex.yy.c $(CFLAGS)

ast: Node.o ast.o quack.tab.o lex.yy.o 
	$(CC) -o quack ast.o quack.tab.o lex.yy.o Node.o  -lfl $(CFLAGS)

run:
	./quack ${QFILE}


clean:
	rm -f *~ quack *.yy.* *.tab.* *.dot *.o
