#pragma once

#define PAGE_SHIFT 12
#define PAGE_SIZE (1 << PAGE_SHIFT) // 4KiB

#define PDT_PAGE_SHIFT 21
#define PDT_PAGE_SIZE (1 << PDT_PAGE_SHIFT) // 2MiB

#define PML4_ALIGN 0x1000
#define PML4_SIZE 0x1000
#define PML4_ENTRY_SIZE 8
#define PML4_IDX_FROM_ADDR(addr) (((addr) >> 39) & 0x1ff)

#define PDPT_ALIGN 0x1000
#define PDPT_SIZE 0x1000
#define PDPT_ENTRY_SIZE 8
#define PDPT_IDX_FROM_ADDR(addr) (((addr) >> 30) & 0x1ff)

#define PAGE_DIRECTORY_ALIGN 0x1000
#define PAGE_DIRECTORY_SIZE 0x1000
#define PAGE_DIRECTORY_ENTRY_SIZE 8
#define PAGE_DIRECTORY_IDX_FROM_ADDR(addr) (((addr) >> 21) & 0x1ff)

#define PAGE_TABLE_ALIGN 0x1000
#define PAGE_TABLE_SIZE 0x1000
#define PAGE_TABLE_ENTRY_SIZE 8
#define PAGE_TABLE_IDX_FROM_ADDR(addr) (((addr) >> 12) & 0x1ff)

#define PAGE_FLAG_PRESENT (1 << 0)
#define PAGE_FLAG_WRITE (1 << 1)
#define PAGE_FLAG_USER (1 << 2)
#define PAGE_FLAG_NX (1 << 63)

#define PDPT_FLAG_1GB (1 << 7)

#define PDT_FLAG_2MB (1 << 7)
