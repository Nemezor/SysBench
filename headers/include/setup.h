#ifndef SETUP_H
#define SETUP_H

#include <multiboot.h>
#include <stdint.h>

extern uint32_t __lnk_kernel_end;

void __setup_kernel_entry(struct multiboot_info* mbinfo, uint32_t bootloader_return);

#endif
