#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gb-repl.h"
#include "parse.h"

void print_state(CpuState* cpu) {
    printf("a: %d f: %d b: %d c: %d d: %d e: %d h: %d l: %d\n", cpu->a, cpu->f, cpu->b, cpu->c, cpu->d, cpu->e, cpu->h, cpu->l);
}

int main() {
    CpuState cpu;
    size_t bufferSize = 20;
    char* inputBuffer = malloc(bufferSize * sizeof(char));

    printf("Welcome to gb-repl. Type 'quit' to exit.\n");
    print_state(&cpu);

    for (;;) {
        printf("> ");
        getline(&inputBuffer, &bufferSize, stdin);

        // Trim the newline.
        inputBuffer[strlen(inputBuffer) - 1] = 0;

        if (strcmp(inputBuffer, "quit") == 0) {
            break;
        }

        const char* error = parse_instruction(&cpu, inputBuffer);
        if (error != NULL) {
            printf("%s\n", error);
        } else {
            print_state(&cpu);
        }
    }

    return 0;
}

