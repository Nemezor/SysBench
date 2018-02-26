#ifndef IO_CPU_PORTS_H
#define IO_CPU_PORTS_H

#include <stdint.h>

#define PORTS_PIC_PRIMARY_COMMAND 0x20
#define PORTS_PIC_PRIMARY_DATA 0x21
#define PORTS_PIC_SECONDARY_COMMAND 0xA0
#define PORTS_PIC_SECONDARY_DATA 0xA1
#define PORTS_PIT_CHANNEL_0 0x40
#define PORTS_PIT_CHANNEL_1 0x41
#define PORTS_PIT_CHANNEL_2 0x42
#define PORTS_PIT_COMMAND 0x43
#define PORTS_PS2_DATA 0x60
#define PORTS_PS2_CMD 0x64
#define PORTS_POST 0x80
#define PORTS_COM1 0x3F8
#define PORTS_PCI_CONFIG_ADDRESS 0xCF8
#define PORTS_PCI_CONFIG_DATA 0xCFC
#define PORTS_VGA_VERTICAL_RETRACE 0x3DA

extern void ports_out8(short port, char data);
extern void ports_out16(short port, short data);
extern void ports_out32(short port, int32_t data);
extern char ports_in8(short port);
extern short ports_in16(short port);
extern int32_t ports_in32(short port);

#endif
