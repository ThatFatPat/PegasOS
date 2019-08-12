OUTPUT_FORMAT(binary)

SECTIONS {
    .bootsect 0x7c00 : AT(0) {
      _start_bootsect = .;
      *(.bootsect)
      . = 510;
      SHORT(0xaa55)
      _end_bootsect = .;
    }
    .text 0x8000 : AT(ADDR(.bootsect) + SIZEOF(.bootsect)) { /* Set address in memory to be 0x8000 but let the sections be sequential in the image file. */
      _start_text = .;
      *(.text)
      _end_text = .;
    }
    .bss: {
      _start_bss = .;
      *(.bss)
      . = ALIGN(8); /* For convenience during zeroing. */
      _end_bss = .;
    }
    .rodata: {
      *(.rodata)
    }
    .data: {
      *(.data)
    }
    _end = .;
    . = ALIGN(512);
    _numsectors = (ADDR(_end) - ADDR(_start_bootsect)) / 512;
}