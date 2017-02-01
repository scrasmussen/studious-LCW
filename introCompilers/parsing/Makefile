BFILE=quack.y
FFILE=quack.l
CC=g++

all: scanner

bison: $(BFILE)
	bison -d $(BFILE)

flex: bison $(FFILE)
	flex $(FFILE)

scanner: flex
	$(CC) quack.tab.c lex.yy.c -lfl -o scanner

clean:
	rm -f *~ scanner *.yy.* *.tab.*
