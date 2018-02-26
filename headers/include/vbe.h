#ifndef VBE_H
#define VBE_H

#include <multiboot.h>
#include <gfx_device.h>

char vbe_initialize(uint8_t bsp_id, struct multiboot_info* mbinfo);
struct gfx_device* vbe_get_gfx_device();

#endif
