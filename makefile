
all: clean debug

debug:
	gcc -Wall -fno-stack-protector src/lexer.c -g -c -o out/lexer.o 
	gcc -Wall -fno-stack-protector src/driver.c -g -c -o out/driver.o
	gcc -Wall -fno-stack-protector -g -o build/pcc out/lexer.o out/driver.o	

firstfollow:
	gcc -Wall -fno-stack-protector src/symbolDef.c -c -o out/symbolDef.o 
	gcc -Wall -fno-stack-protector src/firstAndFollow.c -c -o out/firstAndFollow.o
	gcc -Wall -fno-stack-protector -o build/firstAndFollow out/symbolDef.o out/firstAndFollow.o

parser:
	gcc -Wall -fno-stack-protector src/symbolDef.c -g -c -o out/symbolDef.o 
	gcc -Wall -fno-stack-protector src/parser.c -g -c -o out/parser.o
	gcc -Wall -fno-stack-protector src/symbolStack.c -g -c -o out/symbolStack.o 
	gcc -Wall -fno-stack-protector src/lexer.c -g -c -o out/lexer.o 
	gcc -Wall -fno-stack-protector -g -o build/parser  out/symbolDef.o out/parser.o out/symbolStack.o out/lexer.o
	
run_parser:
	./build/parser language/grammar.txt language/first.txt language/follow.txt language/parseTable.csv testcases/testcase1.txt

clean:
	rm -f build/* out/*

