ENTRY(_start32)

SECTIONS {
  . = 4M;
  _kern_phys_start = .;

  .multiboot ALIGN(8) : {
    KEEP(*(.multiboot))
  }
  .boottext : {
    *(.boottext)
  }
  .bootbss : {
    *(.bootbss)
  }
  .bootrodata : {
    *(.bootrodata)
  }
  .bootdata : {
    *(.bootrodata)
  }
  . = ALIGN(4K);
  _bootseg_end = .;

  . += 0xFFFFFFFF80000000; /* -2GiB */
  _kern_virt_start = .;

  .text ALIGN(4K) : AT(ADDR(.text) - _kern_virt_start + _bootseg_end) {
    *(.text)
  }

  .bss ALIGN(4K) : AT(ADDR(.bss) - _kern_virt_start + _bootseg_end) {
    *(COMMON)
    *(.bss)
  }

  .rodata ALIGN(4K) : AT(ADDR(.rodata) - _kern_virt_start + _bootseg_end) {
    *(.rodata)
  }

  .data ALIGN(4K) : AT(ADDR(.data) - _kern_virt_start + _bootseg_end) {
    *(.data)
  }

  _kern_virt_end = .;
  _kern_phys_end = . - _kern_virt_end + _bootseg_end;
}
