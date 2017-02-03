CC=g++
TESTDIR=/home/faculty/michal/classes/cis461/17W/trial
TESTFILE=quack.input #Sqr.qk ; hands.qk
QFILE=Sqr.qk
QFILE=hands.qk

all: parser run

flex : bison
	flex quack.l #-d

bison : quack.y
	bison -d -t -k -v $< #-g

objs parser.o: flex
	$(CC) -c parser.c quack.tab.c lex.yy.c

parser: parser.o quack.tab.o lex.yy.o
	$(CC) -o parser $^ -lfl

run:
	./parser ${QFILE}

clean:
	rm -f *~ parser *.yy.* *.tab.* *.dot *.o
