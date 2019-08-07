# PegasOS
A simple OS indeed.


How does the boot work:

 - We start in real mode (16-bit)
 - To move to Protected Mode (32-bit)
    - Turn off interrupts
    - Build the GDT

How to use more than 64K on 16-bit:
We use 4-bit segment registers.
  The registers:
  CS - Code Segment
  DS - Data Segment
  SS - Stack Segment
  ES - Extra Segment
  (Later on more were added: FS, GS)
  
Global Descriptor Table (32-bit):
  Added in 32-bit architectures but accessible from 16-bit mode.
  This is the reason we have to turn off interrupts while switching to 32-bit mode from real mode.
  When building it we don't want to be interrupted.
  The GDT register is a register holding an address for the Global Descriptor Table.
  It is a stepping stone between no segments and page files.
