#pragma once

#define GDT_ENTRY_SIZE 8

#define GDT_FLAG_4K (1 << 3)
#define GDT_FLAG_32 (1 << 2)
#define GDT_FLAG_64 (1 << 1)

#define GDT_ACCESS_PRESENT (1 << 7)
#define GDT_ACCESS_RING0 (0 << 5)
#define GDT_ACCESS_RING3 (3 << 5)
#define GDT_ACCESS_NON_SYSTEM (1 << 4)
#define GDT_ACCESS_EXEC (1 << 3)
#define GDT_ACCESS_DIR_DOWN (1 << 2)
#define GDT_ACCESS_READ_WRITE (1 << 1)

// For details, see https://wiki.osdev.org/Global_Descriptor_Table.

#define GDT_ENTRY(base, limit, flags, access)                   \
  (((((base) >> 24) & 0xff) << 56) | (((flags) &0xf) << 52) |   \
   ((((limit) >> 16) & 0xf) << 48) | (((access) &0xff) << 40) | \
   ((((base) >> 16) & 0xfff) << 16) | ((limit) &0xffff))
