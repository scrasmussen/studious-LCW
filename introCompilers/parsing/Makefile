CC=g++
TESTDIR=/home/faculty/michal/classes/cis461/17W/trial
TESTFILE=quack.input #Sqr.qk ; hands.qk

all: parser

flex : bison
	flex -d quack.l

bison : quack.y
	bison -d -t -k -g -v $<

objs parser.o: flex
	$(CC) -c parser.c quack.tab.c lex.yy.c

parser: parser.o quack.tab.o lex.yy.o
	$(CC) -o parser $^ -lfl

clean:
	rm -f *~ parser *.yy.* *.tab.* *.dot *.o
