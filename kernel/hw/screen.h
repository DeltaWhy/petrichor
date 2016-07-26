#ifndef SCREEN_H
#define SCREEN_H

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 26
#define SCROLLBACK_LINES 100
void init_screen();
void update_cursor();
void kclear();
void kputc(char c, char col);
void screen_backspace();
void print(char *str);
void cprint(char *str, char col);
void scroll();
char *itoa( int value, char *str, int base );
extern volatile unsigned char *videoram;
extern volatile unsigned char *cursor;
int printf(const char *format, ...);
#endif
