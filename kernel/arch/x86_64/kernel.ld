ENTRY(_start32)

SECTIONS {
  . = 4M;
  
  _bootseg_start = .;
  .multiboot ALIGN(8) : {
    KEEP(*(.multiboot))
  }
  .boottext : {
    *(.boottext)
  }
  .bootrodata : {
    *(.bootrodata)
  }
  .bootdata : {
    *(.bootdata)
  }
  .bootbss : {
    *(.bootbss)
  }
  . = ALIGN(4K);

  _bootseg_end = .;

  _kern_phys_start = .;
  . += 0xFFFFFFFF80000000; /* -2GiB */
  _kern_virt_start = .;

  _kern_code_start = .;
  .text : AT(ADDR(.text) - _kern_virt_start + _bootseg_end) {
    *(.text)
  }
  . = ALIGN(4K);
  _kern_code_end = .;
  
  _kern_rodata_start = .;
  .rodata : AT(ADDR(.rodata) - _kern_virt_start + _bootseg_end) {
    *(.rodata)
  }
  . = ALIGN(4K);
  _kern_rodata_end = .;

  _kern_data_start = .;
  .data : AT(ADDR(.data) - _kern_virt_start + _bootseg_end) {
    *(.data)
  }
  .bss : AT(ADDR(.bss) - _kern_virt_start + _bootseg_end) {
    *(COMMON)
    *(.bss)
  }
  . = ALIGN(4K);
  _kern_data_end = .;

  _kern_virt_end = .;
  _kern_phys_end = . - _kern_virt_end + _bootseg_end;

  ASSERT(_kern_phys_end < 1024M, "Kernel too large")
}
