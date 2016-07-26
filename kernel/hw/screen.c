#include "port.h"
#include "screen.h"
#include "sprintf.h"
#include "mem/malloc.h"

volatile unsigned char *videoram = (unsigned char *)0xB8000;
volatile unsigned char *cursor = (unsigned char *)0xB8000;
unsigned char scrollback_buf[SCROLLBACK_LINES * SCREEN_WIDTH * 2];
size_t scrollback_start = 0;
size_t scrollback_end = 0;

void init_screen()
{
    for (size_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT * 2; i++) {
        scrollback_buf[i] = videoram[i];
    }
    for (size_t i = SCREEN_WIDTH * SCREEN_HEIGHT * 2; i < sizeof scrollback_buf; i++) {
        scrollback_buf[i] = 0;
    }

    scrollback_end = SCREEN_WIDTH * (SCREEN_HEIGHT-1) * 2;
    cursor = videoram + scrollback_end;
}

void update_cursor()
{
    unsigned short position=(cursor-videoram)/2;

    outportb(0x3D4, 0x0F);
    outportb(0x3D5, (unsigned char)(position&0xFF));

    outportb(0x3D4, 0x0E);
    outportb(0x3D5, (unsigned char)((position>>8)&0xFF));
}

void kclear()
{
    /*cursor = videoram;
    for (int i=0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        cursor[0] = ' ';
        cursor[1] = 0x07;
        cursor += 2;
    }
    cursor = videoram;
    update_cursor();*/
}

/*01 = darkblue 02 = green 03 = lightblue 04 = darkred 05 = darkpink 06 = orange 07 = lightgray 08 = darkgray 09 = purple 10 = babygreen 11 = lightred 12 = lightpink 13 = yellow 14 = white*/
void cprint(char *str, char col)
{
    while (*str != 0) {
        kputc(*str, col);
        scroll();
        update_cursor();
        str++;
    }
}

void print(char *str)
{
    cprint(str, 0x07);
}


void kputc(char c, char col)
{
    scroll();
    int offset, column;
    switch (c) {
    case '\n':
        offset = cursor-videoram;
        column = offset % (SCREEN_WIDTH * 2);
        cursor += (SCREEN_WIDTH * 2) - column;
        scrollback_end += (SCREEN_WIDTH * 2) - column;
        scroll();
        break;
    default:
        cursor[0] = c;
        cursor[1] = col;
        cursor += 2;
        scrollback_buf[scrollback_end] = c;
        scrollback_buf[scrollback_end+1] = col;
        scrollback_end += 2;
    }
    update_cursor();
}

// TODO - this doesn't really belong in this file
void screen_backspace()
{
    if (cursor == videoram) return;
    cursor -= 2;
    cursor[0] = ' ';
    scrollback_end -= 2;
    update_cursor();
}

void scroll()
{
    screen_unscroll();
    // scroll the scrollback buffer
    if (scrollback_end >= sizeof scrollback_buf) {
        // scroll up one line
        unsigned char *dst = scrollback_buf;
        for (size_t i = SCREEN_WIDTH * 2; i < scrollback_end; i++) {
            *dst = scrollback_buf[i];
            dst++;
        }
        // clear last line
        for (size_t i = scrollback_end - SCREEN_WIDTH * 2; i < scrollback_end; i++) {
            scrollback_buf[i] = 0;
        }
        scrollback_end -= SCREEN_WIDTH * 2;
        scrollback_start -= SCREEN_WIDTH * 2;
    }

    int offset = cursor - videoram;
    if (offset >= 160*25) {       /* Are we off-screen ?          */
        scrollback_start += SCREEN_WIDTH * 2;
        for (size_t i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH * 2; i++) {
            videoram[i] = scrollback_buf[scrollback_start+i];
        }
        cursor -= 160;             /* We're on the bottom row      */
    }
}

void screen_scroll_up(void) {
    if (scrollback_start < SCROLL_AMOUNT * SCREEN_WIDTH * 2) {
        scrollback_start = 0;
    } else {
        scrollback_start -= SCROLL_AMOUNT * SCREEN_WIDTH * 2;
    }

    for (size_t i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH * 2; i++) {
        videoram[i] = scrollback_buf[scrollback_start+i];
    }
}

void screen_scroll_down(void) {
    scrollback_start += SCROLL_AMOUNT * SCREEN_WIDTH * 2;
    while (scrollback_start + (SCREEN_HEIGHT-2) * SCREEN_WIDTH * 2 > scrollback_end) {
        scrollback_start -= SCREEN_WIDTH * 2;
    }

    for (size_t i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH * 2; i++) {
        videoram[i] = scrollback_buf[scrollback_start+i];
    }
}

void screen_unscroll(void) {
    if (scrollback_start + (SCREEN_HEIGHT-1) * SCREEN_WIDTH * 2 >= scrollback_end) return;

    while (scrollback_start + (SCREEN_HEIGHT-1) * SCREEN_WIDTH * 2 < scrollback_end) {
        scrollback_start += SCREEN_WIDTH * 2;
    }

    for (size_t i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH * 2; i++) {
        videoram[i] = scrollback_buf[scrollback_start+i];
    }
}

char *itoa( int value, char *str, int base )
{
    char *rc;
    char *ptr;
    char *low;
    // Check for supported base.
    if ( base < 2 || base > 36 ) {
        *str = '\0';
        return str;
    }
    rc = ptr = str;
    // Set '-' for negative decimals.
    if ( value < 0 && base == 10 ) {
        *ptr++ = '-';
    }
    // Remember where the numbers start.
    low = ptr;
    // The actual conversion.
    do {
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while ( value );
    // Terminating the string.
    *ptr-- = '\0';
    // Invert the numbers.
    while ( low < ptr ) {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}

int printf(const char *fmt, ...)
{
    char *buf;
    va_list ap;
    int i;
    va_start(ap, fmt);
    i = vasprintf(&buf, fmt, ap);
    va_end(ap);
    print(buf);
    free(buf);
    return i;
}
