MEMORY
{
   sram_base_addr : ORIGIN = 0x00000000, LENGTH = 0x2000000
   vram1_base_addr : ORIGIN = 0x10200000, LENGTH = 0xF00000
   vram2_base_addr : ORIGIN = 0x11100000, LENGTH = 0xF00000
   vram3_base_addr : ORIGIN = 0x1160000, LENGTH = 0xA00000
   dram_base_addr : ORIGIN = 0x40000000, LENGTH = 0xA00000

   remote_proc_rsc_table : ORIGIN = 0x96600000, LENGTH = 0x1000
   remote_proc_rsc_table_1 : ORIGIN = 0x96650000, LENGTH = 0x1000
   remote_proc_rsc_table_2 : ORIGIN = 0x966a0000, LENGTH = 0x1000
}

ENTRY(_Reset)
