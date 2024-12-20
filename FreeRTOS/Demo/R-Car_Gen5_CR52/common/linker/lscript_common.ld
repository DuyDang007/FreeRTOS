MEMORY
{
   sram_base_addr : ORIGIN = 0x00000000, LENGTH = 0x2000000
   vram_base_addr : ORIGIN = 0x10000000, LENGTH = 0x100000
   remote_proc_rsc_table : ORIGIN = 0x70000000, LENGTH = 0x1000
}

ENTRY(_Reset)
