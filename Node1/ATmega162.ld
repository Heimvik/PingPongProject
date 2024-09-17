MEMORY
{
    FLASH (rx)      : ORIGIN = 0x0000, LENGTH = 16K    /* Flash memory */
    INT_SRAM  (rw)  : ORIGIN = 0x0100, LENGTH = 1K     /* SRAM (Data Memory) */
    EXT_SRAM (rw)   : ORIGIN = 0x1800, LENGTH = 2K
}

SECTIONS
{
    .text :
    {
        *(.vectors)           /* Interrupt Vectors */
        *(.text*)             /* Code Section */
    } > FLASH

    .data :
    {
        *(.data*)             /* Initialized data */
    } > INT_SRAM > FLASH       /* Place data in SRAM but initialize it in Flash */

    .bss :
    {
        *(.bss*)              /* Uninitialized data */
    } > INT_SRAM

    .ext_data :
    {
        *(.ext_data*)
    } > EXTERNAL_SRAM

    .ext_bss :
    {
        *(.ext_bss*)
    } > EXTERNAL_SRAM

    /* Heap and Stack Setup */
    _end = .;                 
    PROVIDE(__heap_start__ = ORIGIN(EXT_SRAM));

    _stack = ORIGIN(INT_SRAM) + LENGTH(INT_SRAM);
    PROVIDE(__stack = _stack); 
}