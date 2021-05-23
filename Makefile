test: build/tests
	./build/tests

build/tests: build/ src/tests.c src/instructions.c
	gcc -o build/tests src/tests.c

build/:
	mkdir build

clean:
	rm -rf build

