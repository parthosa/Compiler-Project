all: clean release

release:
	gcc src/lexer.c -c -o out/lexer.o 
	gcc src/driver.c -c -o out/driver.o
	gcc -o build/pcc out/lexer.o out/driver.o

firstfollow:
	gcc src/firstAndFollow.c -o build/firstAndFollow


clean:
	rm -f build/* out/*

run:
	./build/pcc $(file)