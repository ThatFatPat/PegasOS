/**
 * @addtogroup x86_64
 * @{
 * @file kernel_vspace.h Kernel address space constants.
 */

#pragma once

/**
 * Base address of the kernels address space.
 */
#define ARCH_KERNEL_VSPACE_BASE 0xFFFFFF8000000000 // -512GiB

/**
 * Delta between the kernel's location in physical memory and its location in
 * virtual memory.
 */
#define ARCH_KERNEL_VMA_OFFSET 0xFFFFFFFF80000000 // -2GiB


/**
 * Base address of the mapping of physical memory (the phys map).
 */
#define ARCH_PHYS_MAP_BASE ARCH_KERNEL_VSPACE_BASE

/**
 * Size of the phys map.
 */
#define ARCH_PHYS_MAP_SIZE 0x1000000000 // 64GiB

/** @} */
