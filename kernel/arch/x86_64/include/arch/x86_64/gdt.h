/**
 * @addtogroup x86_64
 * @{
 * @file gdt.h Helpers for managing the Global Descriptor Table. This table is
 * used to define memory segments and their associated permissions. For further
 * details, consult Intel 3a Section 3.2.
 */
#pragma once

/**
 * The size of a GDT entry.
 */
#define X86_64_GDT_ENTRY_SIZE 8

/**
 * Indicate that the segment size has 4KiB- (instead of byte-) granularity.
 */
#define X86_64_GDT_FLAG_4K (1 << 3)

/**
 * Indicate that the segment is a 32-bit (and not 16-bit) segment.
 */
#define X86_64_GDT_FLAG_32 (1 << 2)

/**
 * Indicate that the segment is a 64-bit code segment.
 * @note This flag is only valid when applied to code segments (those marked
 * with @ref X86_64_GDT_ACCESS_EXEC).
 */
#define X86_64_GDT_FLAG_64 (1 << 1)


/**
 * Indicate that the segment is present in memory. If this flag is clear, any
 * attempt to load the segment into a register will result in a
 * segment-not-present exception.
 */
#define X86_64_GDT_ACCESS_PRESENT (1 << 7)

/**
 * Indicate that the segment may only be accessed from ring 0 (kernel mode).
 */
#define X86_64_GDT_ACCESS_RING0 (0 << 5)

/**
 * Indicate that the segment may be accessed from ring 3 and above.
 */
#define X86_64_GDT_ACCESS_RING3 (3 << 5)

/**
 * Indicate that the segment is not a system segment, but a normal code/data
 * segment. For details, see Intel 3a Section 3.5.
 */
#define X86_64_GDT_ACCESS_NON_SYSTEM (1 << 4)

/**
 * Indicate the segment is a code segment (and not a data segment).
 */
#define X86_64_GDT_ACCESS_EXEC (1 << 3)

/**
 * Indicate that the segment grows down (and not up).
 */
#define X86_64_GDT_ACCESS_DIR_DOWN (1 << 2)

/**
 * Segment read/write flag.
 * For code segments, indicate that they are readable as well.
 * For data segments, indicate that they are writeable.
 */
#define X86_64_GDT_ACCESS_READ_WRITE (1 << 1)


/**
 * Construct a GDT entry (segment descriptor) using the provided base, limit,
 * flags and access flags. For details, see Intel 3a Section 3.4.5.
 */
#define X86_64_GDT_ENTRY(base, limit, flags, access)            \
  (((((base) >> 24) & 0xff) << 56) | (((flags) &0xf) << 52) |   \
   ((((limit) >> 16) & 0xf) << 48) | (((access) &0xff) << 40) | \
   ((((base) >> 16) & 0xfff) << 16) | ((limit) &0xffff))

/** @} */
