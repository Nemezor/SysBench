#include <pic.h>
#include <ports.h>
#include <utils.h>
#include <print.h>

#define ICW1_ICW4 0x01
#define ICW1_SINGLE 0x02
#define ICW1_INTERVAL4 0x04
#define ICW1_LEVEL 0x08
#define ICW1_INIT 0x10
#define ICW4_8086 0x01
#define ICW4_AUTO 0x02
#define ICW4_BUF_SLAVE 0x08
#define ICW4_BUF_MASTER 0x0C
#define ICW4_SFNM 0x10

void pic_map(char offset1, char offset2) {
    unsigned char a1, a2;

    printf("[[8259]: Initializing 8259 PICs\n");

    a1 = ports_in8(PORTS_PIC_PRIMARY_DATA);
    a2 = ports_in8(PORTS_PIC_SECONDARY_DATA);

    ports_out8(PORTS_PIC_PRIMARY_COMMAND, ICW1_INIT+ICW1_ICW4);
    iowait();
    ports_out8(PORTS_PIC_SECONDARY_COMMAND, ICW1_INIT+ICW1_ICW4);
    iowait();
    ports_out8(PORTS_PIC_PRIMARY_DATA, offset1);
    iowait();
    ports_out8(PORTS_PIC_SECONDARY_DATA, offset2);
    iowait();
    ports_out8(PORTS_PIC_PRIMARY_DATA, 4);
    iowait();
    ports_out8(PORTS_PIC_SECONDARY_DATA, 2);
    iowait();

    ports_out8(PORTS_PIC_PRIMARY_DATA, ICW4_8086);
    iowait();
    ports_out8(PORTS_PIC_SECONDARY_DATA, ICW4_8086);
    iowait();

    ports_out8(PORTS_PIC_PRIMARY_DATA, a1);
    ports_out8(PORTS_PIC_SECONDARY_DATA, a2);

    printf("[[8259]: PICs Initialized (%u, %u)\n", offset1, offset2);
}

void pic_enable() {
    ports_out8(PORTS_PIC_PRIMARY_DATA, 0);
    ports_out8(PORTS_PIC_SECONDARY_DATA, 0);
    printf("[[8259]: PICs Enabled\n");
}

void pic_disable() {
    ports_out8(PORTS_PIC_PRIMARY_DATA, 0xFF);
    ports_out8(PORTS_PIC_SECONDARY_DATA, 0xFF);
    printf("[[8259]: PICs Disabled\n");
}
