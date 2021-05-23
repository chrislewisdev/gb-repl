#ifndef GBREPL_H
#define GBREPL_H

typedef unsigned short Word;
typedef unsigned char Byte;
typedef unsigned char Register;

typedef struct CpuState {
  Register a;
  Register b;
  Register c;
  Register d;
  Register e;
} CpuState;

#endif

