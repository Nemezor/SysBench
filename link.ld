OUTPUT_FORMAT("elf64-x86-64", "elf64-x86-64", "elf64-x86-64")
ENTRY(__start__)
SECTIONS
{
	. = 0x100000;
	.multiboot : {
		*(.multiboot)
	}
	.text : {
		*(.text)
	}
	.data : {
		*(.data)
	}
	.bss : {
		*(.bss)
	}
	__lnk_kernel_end = ALIGN(4K);
}
