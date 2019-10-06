/**
 * @addtogroup klibc
 * @{
 * @file string.h Partial implementation of the standard &lt;string&gt; header.
 */

#pragma once

#include <stddef.h>

extern "C" {

/**
 * Copy `count` bytes from `src` to `dest`.
 * @note The behavior is undefined if the ranges [`src`, `src + count`) and
 * [`dest`, `dest + count`) overlap. For those cases use @ref ::memmove instead.
 * @return `dest`
 */
void* memcpy(void* __restrict dest, const void* src, size_t count);

/**
 * Copy `count` bytes from `src` to `dest`.
 * @note This function behaves correctly even when [`src`, `src + count`) and
 * [`dest`, `dest + count`) overlap.
 * @return `dest`
 */
void* memmove(void* dest, const void* src, size_t count);

/**
 * Set `count` bytes of the buffer pointed to by `dest` to `ch`.
 * @return `dest`.
 */
void* memset(void* dest, int ch, size_t count);

/**
 * Compute the length of the null-terminated string `str`.
 */
size_t strlen(const char* str);
}


/**
 * Compiler intrinsic version of @ref ::memcpy.
 */
#define memcpy __builtin_memcpy

/**
 * Compiler intrinsic version of @ref ::memmove.
 */
#define memmove __builtin_memmove

/**
 * Compiler intrinsic version of @ref ::memset.
 */
#define memset __builtin_memset

/**
 * Compiler intrinsic version of @ref ::strlen.
 */
#define strlen __builtin_strlen

/** @} */
