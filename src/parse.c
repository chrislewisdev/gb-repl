#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "gb-repl.h"
#include "instructions.h"

Register8* parse_register8(CpuState* cpu, const char* string) {
    if (strcmp(string, "a") == 0) return &cpu->a;
    if (strcmp(string, "b") == 0) return &cpu->b;
    if (strcmp(string, "c") == 0) return &cpu->c;
    if (strcmp(string, "d") == 0) return &cpu->d;
    if (strcmp(string, "e") == 0) return &cpu->e;
    if (strcmp(string, "h") == 0) return &cpu->h;
    if (strcmp(string, "l") == 0) return &cpu->l;

    return NULL;
}

Register16* parse_register16(CpuState* cpu, const char* string) {
    if (strcmp(string, "af") == 0) return (Register16*)&cpu->f;
    if (strcmp(string, "bc") == 0) return (Register16*)&cpu->c;
    if (strcmp(string, "de") == 0) return (Register16*)&cpu->e;
    if (strcmp(string, "hl") == 0) return (Register16*)&cpu->l;

    return NULL;
}

bool is_a(const char* string) {
    return strcmp(string, "a") == 0;
}

bool is_hl_address(const char* string) {
    return strcmp(string, "[hl]") == 0;
}

bool is_zero(const char* string) {
    for (int i = 0; i < strlen(string); i++) {
        if (string[i] != '0') return false;
    }
    return true;
}

int parse_literal(const char* string) {
    // Work out what base the literal is using
    int base = 10;
    if (string[0] == '%') {
        string = string + 1;
        base = 2;
    } else if (string[0] == '$') {
        string = string + 1;
        base = 16;
    }

    // Since strtol returns 0 for non-numbers, make the zero case explicit
    if (is_zero(string)) return 0;

    int value = strtol(string, NULL, base);

    // Ain't no negative values in gameboy land, boy
    if (value < 1) return -1;
    else           return value;
}

byte* parse_memaddress(CpuState* cpu, const char* string) {
    int length = strlen(string);

    // Must always be of the form '[literal|register16]' i.e. minimum length is 3
    if (length < 3 || string[0] != '[' || string[length-1] != ']') {
        return NULL;
    }

    // Pull out the memory address string to parse
    char* address = malloc(length - 2);
    strncpy(address, string + 1, length - 2);

    // Is it a register usage?
    Register16* register16 = parse_register16(cpu, address);
    if (register16 != NULL) {
        return (byte*)&cpu->memory + *register16;
    }

    // Is it a valid memory index?
    int memoryIndex = parse_literal(address);
    if (memoryIndex >= 0 && memoryIndex <= USHRT_MAX) {
        return (byte*)&cpu->memory + memoryIndex;
    } else {
        return NULL;
    }
}

ld8_invocation parse_ld8(CpuState* cpu, const char* destination, const char* source) {
    Register8* destinationRegister = parse_register8(cpu, destination);
    if (destinationRegister != NULL) {
        // Check for format: ld r, r
        Register8* sourceRegister = parse_register8(cpu, source);
        if (sourceRegister != NULL) {
            return (ld8_invocation){.target = destinationRegister, .value = *sourceRegister, .error = NULL};
        }

        // Check for format: ld r, [memaddress]
        // Remembering only a can load from all memaddresses, and only hl can be used with all registers
        if (is_a(destination) || is_hl_address(source)) {
            byte* memoryAddress = parse_memaddress(cpu, source);
            if (memoryAddress != NULL) {
                return (ld8_invocation){.target = destinationRegister, .value = *memoryAddress, .error = NULL};
            }
        }

        // Check for format: ld r, n
        int literal = parse_literal(source);
        if (literal >= 0 && literal <= UCHAR_MAX) {
            return (ld8_invocation){.target = destinationRegister, .value = literal, .error = NULL};
        }
    }

    byte* destinationMemory = parse_memaddress(cpu, destination);
    if (destinationMemory != NULL) {
        // Check format: ld [memaddress], r
        // Remembering only [hl] can load from all registers and only a can load into all addresses
        if (is_hl_address(destination) || is_a(source)) {
            Register8* sourceRegister = parse_register8(cpu, source);
            if (sourceRegister != NULL) {
                return (ld8_invocation){.target = destinationMemory, .value = *sourceRegister, .error = NULL};
            }
        }

        // Check format: ld [hl], n
        int literal = parse_literal(source);
        if (is_hl_address(destination) && literal >= 0 && literal <= UCHAR_MAX) {
            return (ld8_invocation){.target = destinationMemory, .value = literal, .error = NULL};
        }
    }

    return (ld8_invocation){.error = "Correct ld usage: ld register|mem-address, register|mem-address|literal8"};
}
