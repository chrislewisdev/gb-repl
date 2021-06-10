# gb-repl

gb-repl is a [REPL environment](https://en.wikipedia.org/wiki/Read%E2%80%93eval%E2%80%93print_loop) for Gameboy assembly language. That is to say, you can use it to test out assembly instructions in your command line, and see the effects happen before your eyes. It is intended as a learning tool for those new to assembly.

## Current Status

gb-repl is in early development: a basic command-line interface is available, but the interpretation of only some basic instructions has been implemented and tested.

## Building

The project can be built using `make`: at present I have only tested it in a Linux environment but, provided you have `gcc` and `make` installed, you should be able to build on Windows too.

To build and run the command-line interface, use:

```
make run
```

You can run all tests by running:

```
make test
```
