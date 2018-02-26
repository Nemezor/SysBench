#include <print.h>
#include <text_device.h>
#include <stdarg.h>
#include <stdint.h>

extern int64_t double_to_int32_t_int32_t(double value);

struct text_device* print_output;

void print_internal(struct text_device* output, char* format, va_list args);

void print_set_output(struct text_device* device) {
    print_output = device;
}

void putchar(char c, struct text_device* output) {
    if (output != 0 && output->putchar != 0 && output->canprint != 0 && output->canprint()) {
        output->putchar(c);
    }
}

void set_color(char c, struct text_device* output) {
    if (output != 0 && output->setcolor != 0 && output->canprint != 0 && output->canprint()) {
        output->setcolor(c);
    }
}

char get_color(struct text_device* output) {
    if (output != 0 && output->getcolor != 0 && output->canprint != 0 && output->canprint()) {
        return output->getcolor();
    }
    return 0;
}

void print_hex(int64_t num, char adaptive, struct text_device* output) {
    char hit_nonzero = !adaptive;
    for (int i = 15; i >= 0; i--) {
        char c = (char)((num >> (i * 4)) & 0xF);
        if (c < 10) {
            c += 48;
        }else{
            c += 55;
        }
        if (c != '0') {
            hit_nonzero = 1;
        }
        if (hit_nonzero) {
            putchar(c, output);
        }
    }
    if (!hit_nonzero) {
        putchar('0', output);
    }
}

void print_hex_ll(int64_t num, struct text_device* output) {
    for (int i = 15; i >= 0; i--) {
        char c = (char)((num >> (i * 4)) & 0xF);
        if (c < 10) {
            c += 48;
        }else{
            c += 55;
        }
        putchar(c, output);
    }
}

void print_oct(int64_t num, struct text_device* output) {
    for (int i = 24; i >= 0; i--) {
        char c = (char)((num >> (i * 3)) & 0x7);
        c += 48;
        putchar(c, output);
    }
}

void print_dec_u(uint64_t num, struct text_device* output) {
    char buffer[19];
    int counter = 0;
    if (num == 0) {
        putchar('0', output);
        return;
    }
    while (num != 0) {
        char c = num % 10;
        num = num / 10;
        buffer[counter] = c + 48;
        counter++;
    }
    counter--;
    for (; counter >= 0; counter--) {
        putchar(buffer[counter], output);
    }
}

void print_dec_s(int num, struct text_device* output) {
    if (num < 0) {
        putchar('-', output);
        num = -num;
    }
    print_dec_u(num, output);
}

char hex_to_int(char c) {
    if (c > 57) {
        if (c > 70) {
            return c - 87;
        }else{
            return c - 55;
        }
    }else{
        return c - 48;
    }
}

void printf(char* format, ...) {
    va_list args;
    va_start(args, format);
    printf_valist(format, args);
    va_end(args);
}

void printf_valist(char* format, va_list args) {
    print_internal(print_output, format, args);
}

void print_internal(struct text_device* output, char* format, va_list args) {
    int32_t index = 0;
    char c = format[index];
    char special = 0;
    char color = 0;

    while (c != '\0') {
        if (special) {
            if (c == 'c') {
                putchar((char)va_arg(args, unsigned int), output);
            }else if (c == 'd' || c == 'i') {
                print_dec_s(va_arg(args, int), output);
            }else if (c == 'o') {
                print_oct(va_arg(args, int64_t), output);
            }else if (c == 's') {
                char* str = va_arg(args, char*);
                uint32_t i = 0;
                while (1) {
                    if (str[i] == '\0') {
                        break;
                    }else if (str[i] == '\t') {
                        for (int j = 0; j < 4; j++) {
                            putchar(' ', output);
                        }
                    }else{
                        putchar(str[i], output);
                    }
                    i++;
                }
            }else if (c == 'S') {
                char* text = va_arg(args, char*);
                int32_t length = va_arg(args, int32_t);
                for (int32_t i = 0; i < length; i++) {
                    putchar(text[i], output);
                }
            }else if (c == 'u') {
                print_dec_u(va_arg(args, uint64_t), output);
            }else if (c == 'x') {
                print_hex(va_arg(args, int64_t), 1, output);
            }else if (c == 'X') {
                print_hex(va_arg(args, int64_t), 0, output);
            }else if (c == 'p') {
                putchar('0', output);
                putchar('x', output);
                uint64_t address = va_arg(args, uint64_t);
                print_hex((uint32_t)address, 1, output);
            }else if (c == 'P') {
                putchar('0', output);
                putchar('x', output);
                uint64_t address = va_arg(args, uint64_t);
                print_hex(address, 0, output);
            }else if (c == '%') {
                putchar('%', output);
            }else if (c == 'e' || c == 'E' || c == 'g' || c == 'G' || c == 'f' || c == 'F') {
                //double temp = va_arg(args, double);
                //print_hex_ll(double_to_int32_t_int32_t(temp), output);
            }else if (c == 'n') {
                // unsupported
            }else{
                // error
            }
            special = 0;
        }else if (color == 1) {
            if (c == '[') {
                putchar('[', output);
                color = 0;
            }else if (c == 'r') {
                set_color(0x07, output);
                color = 0;
            }else{
                set_color((get_color(output) & 0xF0) | hex_to_int(c), output);
                color = 2;
            }
        }else if (color == 2) {
            set_color((get_color(output) & 0x0F) | (hex_to_int(c) << 4), output);
            color = 0;
        }else{
            if (c == '%') {
                special = 1;
            }else if (c == '[') {
                color = 1;
            }else if (c == '\t') {
                for (char i = 0; i < 4; i++) {
                    putchar(' ', output);
                }
            }else{
                putchar(c, output);
            }
        }

        index++;
        c = format[index];
    }
}
