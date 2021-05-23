#ifndef GBREPL_H
#define GBREPL_H

typedef unsigned short Word;
typedef unsigned char Byte;
typedef unsigned char Register8;
typedef unsigned short Register16;

typedef struct CpuState {
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

