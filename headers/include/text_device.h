#ifndef IO_COMMON_TEXT_DEVICE_H
#define IO_COMMON_TEXT_DEVICE_H

#include <stdint.h>

struct text_device {
    void (*putchar)(char c);
    void (*setcolor)(char c);
    char (*getcolor)(void);
    char (*canprint)(void);
};

#endif
