
all: scanner

flex: quackScanner.lex
	flex quackScanner.lex

scanner: flex
	gcc lex.yy.c -lfl -o scanner

clean:
	rm -f *~ scanner