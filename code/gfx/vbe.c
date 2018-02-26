#include <vbe.h>
#include <multiboot.h>
#include <print.h>
#include <gfx_device.h>
#include <render/software.h>
#include <ports.h>

uint32_t* vbe_framebuffer;
uint32_t vbe_width;
uint32_t vbe_height;
uint32_t vbe_length;
struct gfx_device vbe_gfx_device;

void vbe_update_screen() {
    while (ports_in8(PORTS_VGA_VERTICAL_RETRACE) & 0x08);
    while (!(ports_in8(PORTS_VGA_VERTICAL_RETRACE) & 0x08));
    swrender_update_screen(vbe_framebuffer);
}

char vbe_initialize(uint8_t bsp_id, struct multiboot_info* mbinfo) {
    // LFB
    if (mbinfo->flags & (1 << 12)) {
        // type check
        if (mbinfo->framebuffer_type != 1 || mbinfo->framebuffer_bpp != 32) {
            return 1;
        }
        // color check
        if (mbinfo->framebuffer_red_pos != 16 || mbinfo->framebuffer_red_mask != 8 || mbinfo->framebuffer_green_pos != 8 || mbinfo->framebuffer_green_mask != 8 || mbinfo->framebuffer_blue_pos != 0 || mbinfo->framebuffer_blue_mask != 8) {
            return 1;
        }
        // pitch check
        if (mbinfo->framebuffer_pitch != (mbinfo->framebuffer_width * 4)) {
            return 1;
        }
        printf("[[VBE ]: Framebuffer at %P\n", mbinfo->framebuffer_addr);
        printf("[[VBE ]: Resolution: %ux%u\n", mbinfo->framebuffer_width, mbinfo->framebuffer_height);
        printf("[[VBE ]: Red:    channel %u at %u bits\n", mbinfo->framebuffer_red_pos / 8, mbinfo->framebuffer_red_mask);
        printf("[[VBE ]: Green:  channel %u at %u bits\n", mbinfo->framebuffer_green_pos / 8, mbinfo->framebuffer_green_mask);
        printf("[[VBE ]: Blue:   channel %u at %u bits\n", mbinfo->framebuffer_blue_pos / 8, mbinfo->framebuffer_blue_mask);
        printf("[[VBE ]: Unused: channel %u at %u bits\n", mbinfo->framebuffer_unused_pos / 8, mbinfo->framebuffer_unused_mask);
        printf("[[VBE ]: Pitch: %u BPP: %u\n", mbinfo->framebuffer_pitch, mbinfo->framebuffer_bpp);

        vbe_framebuffer = (uint32_t*)mbinfo->framebuffer_addr;
        vbe_width = mbinfo->framebuffer_width;
        vbe_height = mbinfo->framebuffer_height;
        vbe_length = vbe_height * vbe_width;
        swrender_initialize(bsp_id, vbe_width, vbe_height);
        vbe_gfx_device.update_screen = &vbe_update_screen;
        vbe_gfx_device.renderer = swrender_get_gfx_render();
        vbe_update_screen();
        return 0;
    }
    return 1;
}

struct gfx_device* vbe_get_gfx_device() {
    return &vbe_gfx_device;
}
