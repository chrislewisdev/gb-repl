#ifndef GBREPL_H
#define GBREPL_H

#include <limits.h>

typedef unsigned short word;
typedef unsigned char byte;
typedef unsigned char Register8;
typedef unsigned short Register16;

typedef struct CpuState {
  byte memory[USHRT_MAX];

  Register8 a;
  Register8 f;
  Register8 b;
  Register8 c;
  Register8 d;
  Register8 e;
  Register8 h;
  Register8 l;
} CpuState;

#endif

