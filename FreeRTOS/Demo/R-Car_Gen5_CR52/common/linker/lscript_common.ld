MEMORY
{
   sram_base_addr : ORIGIN = 0x00000000, LENGTH = 0x2000000
   vram1_base_addr : ORIGIN = 0x10100000, LENGTH = 0xB00000
   vram2_base_addr : ORIGIN = 0x10C00000, LENGTH = 0xA00000
   vram3_base_addr : ORIGIN = 0x11600000, LENGTH = 0xA00000
   remote_proc_rsc_table : ORIGIN = 0x80000000, LENGTH = 0x1000
}

ENTRY(_Reset)
