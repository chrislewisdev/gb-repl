#include "gb-repl.h"
#include "instructions.h"

void ld8(ld8_invocation invocation) {
  *invocation.target = invocation.value;
}

