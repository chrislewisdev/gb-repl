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
  return NULL;
}

ld8_invocation parse_ld8(CpuState* cpu, const char* destination, const char* source) {
  Register8* destinationRegister = parse_register8(cpu, destination);
  if (destinationRegister != NULL) {
    Register8* sourceRegister = parse_register8(cpu, source);
    if (sourceRegister != NULL) {
      return (ld8_invocation){.target = destinationRegister, .value = *sourceRegister, .error = NULL};
    }

    int literal = parse_literal(source);
    if (literal >= 0 && literal <= UCHAR_MAX) {
      return (ld8_invocation){.target = destinationRegister, .value = literal, .error = NULL};
    }
  }

  return (ld8_invocation){.error = "Correct ld usage: ld register|mem-address, register|mem-address|literal8"};
}
