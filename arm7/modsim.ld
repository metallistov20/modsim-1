/*** Linker Script File ***/
/*  */

/* Memory Definitions */
/* Atmel AT91SAM7X-EK */
		

MEMORY
{
  IntCodeRAM (rx) : ORIGIN = 0x00000000, LENGTH = 256K
  IntDataRAM (rw) : ORIGIN = 0x00200000, LENGTH = 16k
}



/* Section Definitions */

SECTIONS
{

  /* first section is .text which is used for code */

  .text :
  {
	./arm7/startup.o (.text)         /* Startup code */
	./arm7/inthandles.o (.text)
	./arm7/modsim_main.o (.text)
	./arm7/clock.o (.text)
	./arm7/date_cnt.o (.text)
	./arm7/func.o (.text)
	./arm7/sort.o (.text)
	./arm7/util.o (.text)
    ./*(.text)                   /* remaining code */

    *(.glue_7t) *(.glue_7)

  } >IntCodeRAM =0

  . = ALIGN(4);

  /* .rodata section which is used for read-only data (constants) */

  .rodata . :
  {
    *(.rodata)
  } >IntCodeRAM

  . = ALIGN(4);

  _etext = . ;
  PROVIDE (etext = .);

  /* .data section which is used for initialized data */

  .data : AT (_etext)
  {
    _data = . ;
    *(.data)
    SORT(CONSTRUCTORS)
  } >IntDataRAM
  . = ALIGN(4);

  _edata = . ;
   PROVIDE (edata = .);

  /* .bss section which is used for uninitialized data */

  .bss :
  {
    __bss_start = . ;
    __bss_start__ = . ;
    *(.bss)
    *(COMMON)
  } >IntDataRAM
  . = ALIGN(4);
  __bss_end__ = . ;
  __bss_end__ = . ;

  _end = .;
  PROVIDE (end = .);

  /* Stabs debugging sections.  */
  .stab          0 : { *(.stab) }
  .stabstr       0 : { *(.stabstr) }
  .stab.excl     0 : { *(.stab.excl) }
  .stab.exclstr  0 : { *(.stab.exclstr) }
  .stab.index    0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment       0 : { *(.comment) }
  /* DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to the beginning
     of the section so we begin them at 0.  */
  /* DWARF 1 */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }
  /* GNU DWARF 1 extensions */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }
  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }
  /* SGI/MIPS DWARF 2 extensions */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }
}

