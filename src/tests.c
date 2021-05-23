#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "gb-repl.h"
#include "instructions.c"
#include "parse.c"

#define RUN(TEST) printf(#TEST "..."); TEST(); printf(" passed\n"); 

void test_parse_register() {
  CpuState cpu;

  assert(parse_register8(&cpu, "a") == &cpu.a);
  assert(parse_register8(&cpu, "b") == &cpu.b);
  assert(parse_register8(&cpu, "c") == &cpu.c);
  assert(parse_register8(&cpu, "d") == &cpu.d);
  assert(parse_register8(&cpu, "e") == &cpu.e);
  assert(parse_register8(&cpu, "h") == &cpu.h);
  assert(parse_register8(&cpu, "l") == &cpu.l);
}

void test_ld_r_n_command() {
  CpuState cpu;

  ld_r_n(&cpu.a, 1);
  assert(cpu.a == 1);

  ld_r_n(&cpu.b, 2);
  assert(cpu.b == 2);

  ld_r_n(&cpu.c, 3);
  assert(cpu.c == 3);

  ld_r_n(&cpu.d, 4);
  assert(cpu.d == 4);

  ld_r_n(&cpu.e, 5);
  assert(cpu.e == 5);
}

void test_add_r_r_command() {
  CpuState cpu;
  cpu.a = 1;
  cpu.b = 2;

  add_r_r(&cpu, cpu.a, cpu.b);
  assert(cpu.a == 3);
}

int main() {
  printf("Running tests...\n");
  RUN(test_parse_register);
  RUN(test_ld_r_n_command);
  RUN(test_add_r_r_command);
  printf("Tests complete.\n");
  return 0;
}
