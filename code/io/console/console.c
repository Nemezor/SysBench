#include <console.h>

#define CONSOLE_COLS 80
#define CONSOLE_LINES 25

char* screen_base_ptr = (char*)0xB8000;
char screen_index_x = 0;
char screen_index_y = 0;
char screen_color = 0x07;

void console_check_scroll() {
    if (screen_index_y >= CONSOLE_LINES) {
        for (long i = CONSOLE_COLS * 2; i < CONSOLE_COLS * CONSOLE_LINES * 2; i++) {
            screen_base_ptr[i - CONSOLE_COLS * 2] = screen_base_ptr[i];
        }
        for (long i = CONSOLE_COLS * 2 * (CONSOLE_LINES - 1); i < CONSOLE_COLS * CONSOLE_LINES * 2; i += 2) {
            screen_base_ptr[i] = ' ';
        }
        for (long i = CONSOLE_COLS * 2 * (CONSOLE_LINES - 1) + 1; i < CONSOLE_COLS * CONSOLE_LINES * 2; i += 2) {
            screen_base_ptr[i] = 0x7;
        }
        screen_index_y--;
    }
}

void console_increment_ptrs() {
    screen_index_x++;
    if (screen_index_x >= CONSOLE_COLS) {
        screen_index_x = 0;
        screen_index_y++;
        console_check_scroll();
    }
}

void console_putchar(char c) {
    if (c == '\n') {
        for (long i = screen_index_x; i < CONSOLE_COLS; i++) {
            screen_base_ptr[(screen_index_y * CONSOLE_COLS + i) * 2 + 1] = screen_color;
        }
        screen_index_x = 0;
        screen_index_y++;
        console_check_scroll();
    }else if (c != '\r') {
        screen_base_ptr[(screen_index_y * CONSOLE_COLS + screen_index_x) * 2] = c;
        screen_base_ptr[(screen_index_y * CONSOLE_COLS + screen_index_x) * 2 + 1] = screen_color;
        console_increment_ptrs();
    }
}

void console_setcolor(char c) {
    screen_color = c;
}

char console_getcolor() {
    return screen_color;
}

void console_clear_screen() {
    screen_index_x = 0;
    screen_index_y = 0;
    screen_color = 0x07;
    for (long x = 0; x < CONSOLE_COLS; x++) {
        for (long y = 0; y < CONSOLE_LINES; y++) {
            screen_base_ptr[(y * CONSOLE_COLS + x) * 2] = '\0';
            screen_base_ptr[(y * CONSOLE_COLS + x) * 2 + 1] = screen_color;
        }
    }
}
