#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "gb-repl.h"
#include "instructions.c"

#define RUN(TEST) printf(#TEST "..."); TEST(); printf(" passed\n"); 

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
  RUN(test_ld_r_n_command);
  RUN(test_add_r_r_command);
  printf("Tests complete.\n");
  return 0;
}
