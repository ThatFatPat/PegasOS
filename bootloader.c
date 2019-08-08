#include <stdint.h>
#ifndef asm(x)
#define asm(x)(__asm__ __volatile__(x))
#endif
