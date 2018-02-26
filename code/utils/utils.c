#include <utils.h>
#include <ports.h>
#include <stdint.h>

void iowait() {
    ports_out8(PORTS_POST, 0xFF);
}

uint32_t uint2strd(uint64_t num, char* str) {
    char buffer[19];
    int counter = 0;
    if (num == 0) {
        str[0] = '0';
        str[1] = '\n';
        return 1;
    }
    while (num != 0) {
        char c = num % 10;
        num = num / 10;
        buffer[counter] = c + 48;
        counter++;
    }
    counter--;
    int i = 0;
    for (; counter >= 0; counter--) {
        str[i] = buffer[counter];
        i++;
    }
    str[i] = '\0';
    return i;
}

uint32_t int2strd(int64_t num, char* str) {
    int offset = 0;
    if (((num >> 63) & 1)) {
        str[0] = '-';
        num = ~num + 1;
        offset = 1;
    }
    return uint2strd(num, str + offset) + offset;
}

uint32_t int2strh(int64_t num, char* str) {
    return uint2strh((uint64_t)num, str);
}

uint32_t uint2strh(uint64_t num, char* str) {
    int index = 0;
    for (int i = 15; i >= 0; i--) {
        char c = (char)((num >> (i * 4)) & 0xF);
        if (c < 10) {
            c += 48;
        }else{
            c += 55;
        }
        str[index] = c;
        index++;
    }
    str[index] = '\0';
    return index;
}
