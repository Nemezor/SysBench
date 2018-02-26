#include <serial.h>
#include <ports.h>
#include <print.h>
#include <stdint.h>

struct text_device serial_output;

void serial_putchar(char c) {
    while ((ports_in8(PORTS_COM1 + 5) & 0x20) == 0);

    if (c == '\n') {
        ports_out8(PORTS_COM1, '\r');
    }
    ports_out8(PORTS_COM1, c);
}

void serial_setcolor(char c) {

}

char serial_getcolor() {
    return 0;
}

char serial_canprint() {
    return 1;
}

void serial_initialize() {
    ports_out8(PORTS_COM1 + 1, 0x00);
    ports_out8(PORTS_COM1 + 3, 0x80);
    ports_out8(PORTS_COM1 + 0, 0x01);
    ports_out8(PORTS_COM1 + 1, 0x00);
    ports_out8(PORTS_COM1 + 3, 0x03);
    ports_out8(PORTS_COM1 + 2, 0xC7);
    ports_out8(PORTS_COM1 + 4, 0x0B);

    serial_output.putchar = &serial_putchar;
    serial_output.setcolor = &serial_setcolor;
    serial_output.getcolor = &serial_getcolor;
    serial_output.canprint = &serial_canprint;
}

struct text_device* serial_get_text_device() {
    return &serial_output;
}
