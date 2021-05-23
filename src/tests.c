#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "gb-repl.h"
#include "instructions.c"
#include "parse.c"

#define RUN(TEST) printf(#TEST "..."); TEST(); printf(" passed\n"); 

void test_parse_register8() {
  CpuState cpu;

  assert(parse_register8(&cpu, "a") == &cpu.a);
  assert(parse_register8(&cpu, "b") == &cpu.b);
  assert(parse_register8(&cpu, "c") == &cpu.c);
  assert(parse_register8(&cpu, "d") == &cpu.d);
  assert(parse_register8(&cpu, "e") == &cpu.e);
  assert(parse_register8(&cpu, "h") == &cpu.h);
  assert(parse_register8(&cpu, "l") == &cpu.l);

  assert(parse_register8(&cpu, "gibberish") == NULL);
}

void test_parse_ld8() {
  CpuState cpu;
  byte valueToSet = 5;
  cpu.b = valueToSet;

  ld8_invocation invocation = parse_ld8(&cpu, "a", "b");
  assert(invocation.target == &cpu.a);
  assert(invocation.value == valueToSet);
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
  RUN(test_parse_ld8);
  RUN(test_ld8_command);
  printf("Tests complete.\n");
  return 0;
}
