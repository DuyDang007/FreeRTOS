MEMORY
{
   sram_base_addr : ORIGIN = 0x10000000, LENGTH = 0x200000
   vram_base_addr : ORIGIN = 0x00000000, LENGTH = 0x2000000
}

ENTRY(_Reset)
