SECTIONS {
	. = 0x20000000;
	.text : {
	  KEEP(*(.app_sign))
	  KEEP(*(.app))
	  *(.text)
  }
  . = 0x20001800;
  .data : {
    *(.data)
  }
  .bss : {
    *(.bss)
  }
}
