FILE=quack.l

all: scanner

flex: $(FILE)
	flex $(FILE)

scanner: flex
	gcc lex.yy.c -lfl -o scanner

clean:
	rm -f *~ scanner
