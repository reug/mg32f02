SECTIONS {
	. = 0x1000;
	.text : {
	  KEEP(*(.app_sign))
	  KEEP(*(.app))
	  *(.text)
  }
  . = 0x20001000;
  .data : {
    *(.data)
  }
  .bss : {
    *(.bss)
  }
  . = 0x20001C00;
  .dma (NOLOAD): {
    KEEP(*(.eth_frame))
  }
}
