SECTIONS
{
.resource_table : {
    __resource_table_start = .;
    . = ALIGN(4);
    KEEP(*(.resource_table))
    __resource_table_end = .;
} > remote_proc_rsc_table

.resource_table1 : {
    __resource_table1_start = .;
    . = ALIGN(4);
    KEEP(*(.resource_table1))
    __resource_table1_end = .;
} > remote_proc_rsc_table_1

.resource_table2 : {
    __resource_table2_start = .;
    . = ALIGN(4);
    KEEP(*(.resource_table2))
    __resource_table2_end = .;
} > remote_proc_rsc_table_2

}
