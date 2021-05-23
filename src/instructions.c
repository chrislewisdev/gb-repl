#include "gb-repl.h"

void ld_r_n(Register8* r, Byte value) {
  *r = value;
}

void add_r_r(CpuState *cpu, Register8 r1, Register8 r2) {
  cpu->a = r1 + r2;
}

