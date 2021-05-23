#include "gb-repl.h"

void ld_r_n(Register* r, Byte value) {
  *r = value;
}

void add_r_r(CpuState *cpu, Register r1, Register r2) {
  cpu->a = r1 + r2;
}

