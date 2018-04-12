
all: debug

debug:clean
	gcc -Wall -fno-stack-protector src/lexer.c -g -c -o out/lexer.o 
	gcc -Wall -fno-stack-protector src/symbolDef.c -g -c -o out/symbolDef.o 
	gcc -Wall -fno-stack-protector src/firstAndFollow.c -c -o out/firstAndFollow.o
	gcc -Wall -fno-stack-protector src/parserDef.c -g -c -o out/parserDef.o 
	gcc -Wall -fno-stack-protector src/symbolStack.c -g -c -o out/symbolStack.o 
	gcc -Wall -fno-stack-protector src/parser.c -g -c -o out/parser.o
	gcc -Wall -fno-stack-protector src/ast.c -g -c -o out/ast.o
	gcc -Wall -fno-stack-protector src/driver.c -g -c -o out/driver.o
	gcc -Wall -fno-stack-protector -g -o build/pcc out/driver.o out/lexer.o out/symbolDef.o out/firstAndFollow.o out/parser.o out/parserDef.o out/symbolStack.o out/ast.o

firstfollow:clean
	gcc -Wall -fno-stack-protector src/symbolDef.c -c -o out/symbolDef.o 
	gcc -Wall -fno-stack-protector src/firstAndFollow.c -c -o out/firstAndFollow.o
	gcc -Wall -fno-stack-protector -o build/firstAndFollow out/symbolDef.o out/firstAndFollow.o

	
run_firstfollow:
	./build/firstAndFollow language/hr.txt language/hr_first.txt language/hr_follow.txt
	


parser: clean
	gcc -Wall -fno-stack-protector src/symbolDef.c -g -c -o out/symbolDef.o 
	gcc -Wall -fno-stack-protector src/parserDef.c -g -c -o out/parserDef.o 
	gcc -Wall -fno-stack-protector src/parser.c -g -c -o out/parser.o
	gcc -Wall -fno-stack-protector src/symbolStack.c -g -c -o out/symbolStack.o 
	gcc -Wall -fno-stack-protector src/lexer.c -g -c -o out/lexer.o 
	gcc -Wall -fno-stack-protector -g -o build/parser out/parser.o out/symbolDef.o out/parserDef.o out/symbolStack.o out/lexer.o
	
run_parser:
	./build/parser language/grammar.txt language/first.txt language/follow.txt language/parseTable.csv testcases/testcase3.txt language/parseTree.txt

gdb_run_parser:
	gdb --args ./build/parser language/grammar.txt language/first.txt language/follow.txt language/parseTable.csv testcases/testcase3.txt language/parseTree.txt
	

run:
	./build/pcc testcases/test1.txt

gdb_run:
	gdb --args ./build/pcc testcases/test0.txt

release:
	cp src/* 2015A7PS0088P/

clean:
	rm -f build/* out/*

