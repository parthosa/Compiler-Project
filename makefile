all: clean release

release:
	gcc src/lexer.c -w -c -o out/lexer.o 
	gcc src/driver.c -w -c -o out/driver.o
	gcc -w -o build/pcc out/lexer.o out/driver.o

clean:
	rm -f build/* out/*

run:
	./build/pcc $(file)