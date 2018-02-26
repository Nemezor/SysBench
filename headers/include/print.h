#ifndef IO_COMMON_PRINT_H
#define IO_COMMON_PRINT_H

#include <text_device.h>
#include <stdint.h>
#include <stdarg.h>

/**
 * Prints formatted text into console
 *
 * %u - uint32_t
 * %i, %d - signed int32_t
 * %o - octal
 * %x - adaptive-size hexadecimal
 * %X - fixed-size hexadecimal
 * %c - character
 * %s - string
 * %S - string with set length (next argument = length)
 * %p - lower-case pointer
 * %P - upper-case pointer
 * %% - literal '%'
 * [## - color code (standard VGA textmode colors)
 *  |^- hexadecimal background color
 *  ^-- hexadecimal foreground color
 * [r - reset colors
 * [[ - literal '['
**/
void printf(char* format, ...);
void printf_valist(char* format, va_list args);
void print_set_output(struct text_device* device);

#endif
