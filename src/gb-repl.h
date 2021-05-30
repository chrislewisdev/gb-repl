#ifndef GBREPL_H
#define GBREPL_H

#include <limits.h>

typedef unsigned short word;
typedef unsigned char byte;
typedef unsigned char Register8;
typedef unsigned short Register16;

typedef struct CpuState {
  byte memory[USHRT_MAX];

  // Each register pair is specified in reverse to achieve what I think is the correct endian-ness
  // I may be wrong and as such might need to reverse this decision later....
  Register8 f;
  Register8 a;
  Register8 c;
  Register8 b;
  Register8 e;
  Register8 d;
  Register8 l;
  Register8 h;
} CpuState;

#endif

