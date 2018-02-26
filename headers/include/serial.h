#ifndef IO_COMMON_SERIAL_H
#define IO_COMMON_SERIAL_H

#include "text_device.h"
#include <stdint.h>

void serial_initialize();
struct text_device* serial_get_text_device();

#endif
