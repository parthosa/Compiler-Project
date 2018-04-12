
CFLAGS = -Wall -fno-stack-protector -g -c

all: clean release

release:
	gcc $(CFLAGS) lexer.c -o lexer.o 
	gcc $(CFLAGS) symbolDef.c -o symbolDef.o 
	gcc $(CFLAGS) firstAndFollow.c -c -o firstAndFollow.o
	gcc $(CFLAGS) parserDef.c -o parserDef.o 
	gcc $(CFLAGS) symbolStack.c -o symbolStack.o 
	gcc $(CFLAGS) parser.c -o parser.o
	gcc $(CFLAGS) driver.c -o driver.o
	gcc -g -o stage1exe driver.o lexer.o symbolDef.o firstAndFollow.o parser.o parserDef.o symbolStack.o


clean:
	rm -f stage1exe *.o

