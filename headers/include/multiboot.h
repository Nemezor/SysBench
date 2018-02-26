#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <stdint.h>

     /* multiboot.h - the header for Multiboot */
     /* Copyright (C) 1999, 2001  Free Software Foundation, Inc.

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program; if not, write to the Free Software
        Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. */

     /* Macros. */

     /* The magic number for the Multiboot header. */
     #define MULTIBOOT_HEADER_MAGIC          0x1BADB002

     /* The flags for the Multiboot header. */
     #ifdef __ELF__
     # define MULTIBOOT_HEADER_FLAGS         0x00000003
     #else
     # define MULTIBOOT_HEADER_FLAGS         0x00010003
     #endif

     /* The magic number passed by a Multiboot-compliant boot loader. */
     #define MULTIBOOT_BOOTLOADER_MAGIC      0x2BADB002

     /* The size of our stack (16KB). */
     #define STACK_SIZE                      0x4000

     /* C symbol format. HAVE_ASM_USCORE is defined by configure. */
     #ifdef HAVE_ASM_USCORE
     # define EXT_C(sym)                     _ ## sym
     #else
     # define EXT_C(sym)                     sym
     #endif

     #ifndef ASM
     /* Do not include here in boot.S. */

     /* Types. */

     /* The Multiboot header. */
     struct multiboot_header
     {
       uint32_t magic;
       uint32_t flags;
       uint32_t checksum;
       uint32_t header_addr;
       uint32_t load_addr;
       uint32_t load_end_addr;
       uint32_t bss_end_addr;
       uint32_t entry_addr;
     };

     /* The symbol table for a.out. */
     struct aout_symbol_table
     {
       uint32_t tabsize;
       uint32_t strsize;
       uint32_t addr;
       uint32_t reserved;
     };

     /* The section header table for ELF. */
     struct elf_section_header_table
     {
       uint32_t num;
       uint32_t size;
       uint32_t addr;
       uint32_t shndx;
     };

     /* The Multiboot information. */
     struct multiboot_info
     {
       uint32_t flags;
       uint32_t mem_lower;
       uint32_t mem_upper;
       uint32_t boot_device;
       uint32_t cmdline;
       uint32_t mods_count;
       uint32_t mods_addr;
       union
       {
         struct aout_symbol_table aout_sym;
         struct elf_section_header_table elf_sec;
       } u;
       uint32_t mmap_length;
       uint32_t mmap_addr;
       uint32_t drives_length;
       uint32_t drives_addr;
       uint32_t config_table;
       uint32_t boot_loader_name;
       uint32_t apm_table;
       uint32_t vbe_control_info;
       uint32_t vbe_mode_info;
       unsigned short vbe_mode;
       unsigned short vbe_interface_seg;
       unsigned short vbe_interface_off;
       unsigned short vbe_interface_len;
       uint64_t framebuffer_addr;
       uint32_t framebuffer_pitch;
       uint32_t framebuffer_width;
       uint32_t framebuffer_height;
       uint8_t framebuffer_bpp;
       uint8_t framebuffer_type;
       uint8_t framebuffer_unused_pos;
       uint8_t framebuffer_unused_mask;
       uint8_t framebuffer_red_pos;
       uint8_t framebuffer_red_mask;
       uint8_t framebuffer_green_pos;
       uint8_t framebuffer_green_mask;
       uint8_t framebuffer_blue_pos;
       uint8_t framebuffer_blue_mask;
     };

     /* The module structure. */
     struct module
     {
       uint32_t mod_start;
       uint32_t mod_end;
       uint32_t string;
       uint32_t reserved;
     };

     /* The memory map. Be careful that the offset 0 is base_addr_low
        but no size. */
     struct memory_map
     {
       uint32_t size;
       uint32_t base_addr_low;
       uint32_t base_addr_high;
       uint32_t length_low;
       uint32_t length_high;
       uint32_t type;
     };

     #endif /* ! ASM */
#endif
