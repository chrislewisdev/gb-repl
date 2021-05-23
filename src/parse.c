#include <string.h>

#include "gb-repl.h"

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
