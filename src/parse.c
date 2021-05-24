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

ld8_invocation parse_ld8(CpuState* cpu, const char* destination, const char* source) {
  Register8* destinationRegister = parse_register8(cpu, destination);
  if (destinationRegister != NULL) {
    Register8* sourceRegister = parse_register8(cpu, source);
    if (sourceRegister != NULL) {
      return (ld8_invocation){.target = destinationRegister, .value = *sourceRegister, .error = NULL};
    }
  }

  return (ld8_invocation){.error = "Correct ld usage: ld register|mem-address, register|mem-address|literal"};
}

