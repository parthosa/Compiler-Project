
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
	gcc src/symbolDef.c -g -c -o out/symbolDef.o 
	gcc src/parser.c -g -c -o out/parser.o
	gcc src/symbolStack.c -g -c -o out/symbolStack.o 
	gcc src/lexer.c -g -c -o out/lexer.o 
	gcc -g -o build/parser  out/symbolDef.o out/parser.o out/symbolStack.o out/lexer.o
	
run_parser:
	./build/parser language/grammar.txt language/first.txt language/follow.txt language/parseTable.csv testcases/test1.txt

clean:
	rm -f build/* out/*

