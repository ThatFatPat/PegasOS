/**
 * @addtogroup x86_64
 * @{
 * @file cpuid.h CPU feature and identification helpers. */

#pragma once

/**
 * Retrieve maximum supported extended `cpuid` function.
 */
#define X86_64_CPUID_FUNC_EXT_CPUID 0x80000000

/**
 * Retrieve extended processor info.
 */
#define X86_64_CPUID_FUNC_EXT_PROC_INFO 0x80000001


/**
 * Indicates CPU support for long mode when set in the `edx` register of `cpuid`
 * page @ref X86_64_CPUID_FUNC_EXT_PROC_INFO.
 */
#define X86_64_CPU_FEATURE_LONG_MODE (1 << 29)

/** @} */
