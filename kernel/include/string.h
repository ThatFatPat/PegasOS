#pragma once

#include <stddef.h>

extern "C" {
void* memcpy(void* __restrict dest, const void* src, size_t count);
void* memmove(void* dest, const void* src, size_t count);
void* memset(void* dest, int ch, size_t count);
size_t strlen(const char* str);
}

// Use GCC's intrinsics for these functions
#define memcpy __builtin_memcpy
#define memmove __builtin_memmove
#define memset __builtin_memset
#define strlen __builtin_strlen
