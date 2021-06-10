#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

typedef struct ld8_invocation {
    byte* target;
    byte value;
    const char* error;
} ld8_invocation;

void ld8(ld8_invocation);

#endif

