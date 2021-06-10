run: build/gb-repl
	./build/gb-repl

build/gb-repl: src/main.c src/parse.h src/parse.c src/instructions.h src/instructions.c
	gcc -o build/gb-repl src/main.c src/parse.c src/instructions.c

test: build/tests
	./build/tests

build/tests: build/ src/tests.c src/instructions.c src/parse.c
	gcc -o build/tests src/tests.c

build/:
	mkdir build

clean:
	rm -rf build

