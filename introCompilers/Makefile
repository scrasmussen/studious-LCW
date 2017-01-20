FILE=quack.l
CC=g++

all: scanner

flex: $(FILE)
	flex $(FILE)

scanner: flex
	$(CC) lex.yy.c -lfl -o scanner

clean:
	rm -f *~ scanner
