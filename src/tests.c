#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "gb-repl.h"
#include "instructions.c"
#include "parse.c"

#define RUN(TEST) printf(#TEST "..."); TEST(); printf(" passed\n");
// TODO: Redefine assert to allow all tests to run despite failures?
// #define assert(expr) if (!(expr)) { printf("Line %d: '%s' was false", __LINE__, #expr); return; }

void test_parse_literal() {
    assert(parse_literal("0") == 0);
    assert(parse_literal("50") == 50);
    assert(parse_literal("123") == 123);
    assert(parse_literal("255") == 255);

    assert(parse_literal("%01") == 1);
    assert(parse_literal("%000010") == 2);
    assert(parse_literal("%00000000") == 0);
    assert(parse_literal("%1010") == 10);

    assert(parse_literal("$08") == 8);
    assert(parse_literal("$0F") == 15);
    assert(parse_literal("$FF") == 255);
    assert(parse_literal("$00") == 0);
    assert(parse_literal("$F0") == 240);

    assert(parse_literal("-1") == -1);
    assert(parse_literal("-5") == -1);
    assert(parse_literal("a") == -1);
    assert(parse_literal("[$0800]") == -1);
}

void test_parse_register8() {
    CpuState cpu;

    assert(parse_register8(&cpu, "a") == &cpu.a);
    assert(parse_register8(&cpu, "b") == &cpu.b);
    assert(parse_register8(&cpu, "c") == &cpu.c);
    assert(parse_register8(&cpu, "d") == &cpu.d);
    assert(parse_register8(&cpu, "e") == &cpu.e);
    assert(parse_register8(&cpu, "h") == &cpu.h);
    assert(parse_register8(&cpu, "l") == &cpu.l);

    assert(parse_register8(&cpu, "f") == NULL);
    assert(parse_register8(&cpu, "gibberish") == NULL);
}

void test_parse_register16() {
    CpuState cpu;

    assert(parse_register16(&cpu, "af") == (word*)&cpu.f);
    assert(parse_register16(&cpu, "bc") == (word*)&cpu.c);
    assert(parse_register16(&cpu, "de") == (word*)&cpu.e);
    assert(parse_register16(&cpu, "hl") == (word*)&cpu.l);

    assert(parse_register16(&cpu, "") == NULL);
    assert(parse_register16(&cpu, "a") == NULL);
    assert(parse_register16(&cpu, "gibberish") == NULL);
}

void test_parse_memaddress() {
    CpuState cpu;

    assert(parse_memaddress(&cpu, "[0]") == (byte*)&cpu.memory);
    assert(parse_memaddress(&cpu, "[65535]") == (byte*)&cpu.memory + 65535);
    assert(parse_memaddress(&cpu, "[$FF]") == (byte*)&cpu.memory + 255);
    assert(parse_memaddress(&cpu, "[%1010]") == (byte*)&cpu.memory + 10);

    assert(parse_memaddress(&cpu, "[-1]") == NULL);
    assert(parse_memaddress(&cpu, "[65536]") == NULL);
    assert(parse_memaddress(&cpu, "") == NULL);
    assert(parse_memaddress(&cpu, "[]") == NULL);
}

void test_parse_memaddress_register16() {
    CpuState cpu;

    cpu.b = 0;
    cpu.c = 0;
    assert(parse_memaddress(&cpu, "[bc]") == (byte*)&cpu.memory);

    cpu.b = 0;
    cpu.c = 10;
    assert(parse_memaddress(&cpu, "[bc]") == (byte*)&cpu.memory + 10);

    // bc = $0F00 (3840 in base 10)
    cpu.b = 15;
    cpu.c = 0;
    assert(parse_memaddress(&cpu, "[bc]") == (byte*)&cpu.memory + 3840);

    // bc = $FFFF
    cpu.b = 255;
    cpu.c = 255;
    assert(parse_memaddress(&cpu, "[bc]") == (byte*)&cpu.memory + 65535);
}

void test_parse_ld_r_r() {
    CpuState cpu;
    byte valueToSet = 5;
    cpu.b = valueToSet;

    ld8_invocation invocation = parse_ld8(&cpu, "a", "b");
    assert(invocation.error == NULL);
    assert(invocation.target == &cpu.a);
    assert(invocation.value == valueToSet);
}

void test_parse_ld_r_n() {
    CpuState cpu;

    ld8_invocation invocation = parse_ld8(&cpu, "b", "10");
    assert(invocation.error == NULL);
    assert(invocation.target == &cpu.b);
    assert(invocation.value == 10);

    invocation = parse_ld8(&cpu, "b", "256");
    assert(invocation.error != NULL);
}

void test_parse_ld_r_hl() {
    // Setup hl = 1234, [hl] = 123
    CpuState cpu;
    word memoryAddress = 1234;
    byte value = 123;
    cpu.memory[memoryAddress] = value;
    *((word*)&cpu.l) = memoryAddress;

    const char registers[5][2] = {"a", "b", "c", "d", "e"};

    for (int i = 0; i < 5; i++) {
        ld8_invocation invocation = parse_ld8(&cpu, registers[i], "[hl]");
        assert(invocation.error == NULL);
        assert(invocation.target == parse_register8(&cpu, registers[i]));
        assert(invocation.value == value); 
    }
}

void test_parse_ld_hl_r() {
    // Setup hl = 1234, [hl] = 0
    CpuState cpu;
    word memoryAddress = 1234;
    cpu.memory[memoryAddress] = 0;
    *((word*)&cpu.l) = memoryAddress;

    const char registers[5][2] = {"a", "b", "c", "d", "e"};
    cpu.a = 1;
    cpu.b = 1;
    cpu.c = 1;
    cpu.d = 1;
    cpu.e = 1;

    for (int i = 0; i < 5; i++) {
        ld8_invocation invocation = parse_ld8(&cpu, "[hl]", registers[i]);
        assert(invocation.error == NULL);
        assert(invocation.target == &cpu.memory[memoryAddress]);
        assert(invocation.value == 1);
    }
}

void test_parse_ld_hl_n() {
    CpuState cpu;
    word memoryAddress = 1234;
    *((word*)&cpu.l) = memoryAddress;

    ld8_invocation invocation = parse_ld8(&cpu, "[hl]", "98");
    assert(invocation.error == NULL);
    assert(invocation.target == &cpu.memory[memoryAddress]);
    assert(invocation.value == 98);
}

// TODO: Use this more widely...
bool check_ld8(ld8_invocation invocation, byte* target, byte value) {
    return invocation.error == NULL &&
        invocation.target == target &&
        invocation.value == value;
}

void test_parse_ld_memaddress_a() {
    CpuState cpu;
    byte valueToSet = 5;
    word memoryAddress = 1234;
    cpu.memory[memoryAddress] = valueToSet;
    *((word*)&cpu.c) = memoryAddress;
    *((word*)&cpu.e) = memoryAddress;

    assert(check_ld8(parse_ld8(&cpu, "a", "[bc]"), &cpu.a, valueToSet));
    assert(check_ld8(parse_ld8(&cpu, "a", "[de]"), &cpu.a, valueToSet));
    assert(check_ld8(parse_ld8(&cpu, "a", "[1234]"), &cpu.a, valueToSet));

    cpu.a = valueToSet;
    assert(check_ld8(parse_ld8(&cpu, "[bc]", "a"), &cpu.memory[memoryAddress], valueToSet));
    assert(check_ld8(parse_ld8(&cpu, "[de]", "a"), &cpu.memory[memoryAddress], valueToSet));
    assert(check_ld8(parse_ld8(&cpu, "[1234]", "a"), &cpu.memory[memoryAddress], valueToSet));
}

void test_parse_ld_errors() {
    CpuState cpu;

    ld8_invocation invocation = parse_ld8(&cpu, "5", "a");
    assert(invocation.error != NULL);

    invocation = parse_ld8(&cpu, "b", "[de]");
    assert(invocation.error != NULL);

    invocation = parse_ld8(&cpu, "[de]", "b");
    assert(invocation.error != NULL);

    invocation = parse_ld8(&cpu, "[150]", "b");
    assert(invocation.error != NULL);

    invocation = parse_ld8(&cpu, "b", "[150]");
    assert(invocation.error != NULL);

    // TODO: Assert that register16 usages and register8 usages can't clash
}

void test_ld8_command() {
    CpuState cpu;

    ld8((ld8_invocation){.target = &cpu.a, .value = 1});
    assert(cpu.a == 1);

    ld8((ld8_invocation){.target = &cpu.b, .value = 2});
    assert(cpu.b == 2);

    ld8((ld8_invocation){.target = &cpu.c, .value = 3});
    assert(cpu.c == 3);

    ld8((ld8_invocation){.target = &cpu.d, .value = 4});
    assert(cpu.d == 4);

    ld8((ld8_invocation){.target = &cpu.e, .value = 5});
    assert(cpu.e == 5);
}

int main() {
    printf("Running tests...\n");
    RUN(test_parse_register8);
    RUN(test_parse_register16);
    RUN(test_parse_literal);
    RUN(test_parse_memaddress);
    RUN(test_parse_memaddress_register16);
    RUN(test_parse_ld_r_r);
    RUN(test_parse_ld_r_n);
    RUN(test_parse_ld_r_hl);
    RUN(test_parse_ld_hl_r);
    RUN(test_parse_ld_hl_n);
    RUN(test_parse_ld_memaddress_a);
    RUN(test_parse_ld_errors);
    RUN(test_ld8_command);
    printf("Tests complete.\n");
    return 0;
}
