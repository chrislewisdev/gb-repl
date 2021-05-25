#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "gb-repl.h"
#include "instructions.c"
#include "parse.c"

#define RUN(TEST) printf(#TEST "..."); TEST(); printf(" passed\n");

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

void test_parse_ld8() {
  CpuState cpu;
  byte valueToSet = 5;
  cpu.b = valueToSet;

  // Test ld r, r
  ld8_invocation invocation = parse_ld8(&cpu, "a", "b");
  assert(invocation.error == NULL);
  assert(invocation.target == &cpu.a);
  assert(invocation.value == valueToSet);

  // Test ld r, n
  invocation = parse_ld8(&cpu, "b", "10");
  assert(invocation.error == NULL);
  assert(invocation.target == &cpu.b);
  assert(invocation.value == 10);

  invocation = parse_ld8(&cpu, "b", "256");
  assert(invocation.error != NULL);

  // Test invalid case
  invocation = parse_ld8(&cpu, "5", "a");
  assert(invocation.error != NULL);
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
  RUN(test_parse_literal);
  RUN(test_parse_ld8);
  RUN(test_ld8_command);
  printf("Tests complete.\n");
  return 0;
}
