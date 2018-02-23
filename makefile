
all: clean debug

debug:
	gcc src/lexer.c -c -o out/lexer.o 
	gcc src/driver.c -c -o out/driver.o
	gcc -o build/pcc out/lexer.o out/driver.o	

firstfollow:
	gcc src/symbolDef.c -c -o out/symbolDef.o 
	gcc src/firstAndFollow.c -c -o out/firstAndFollow.o
	gcc -o build/firstAndFollow out/symbolDef.o out/firstAndFollow.o

parser:
	gcc src/symbolDef.c -c -o out/symbolDef.o 
	gcc src/parser.c -c -o out/parser.o
	gcc -o build/parser  out/symbolDef.o out/parser.o
	

clean:
	rm -f build/* out/*

