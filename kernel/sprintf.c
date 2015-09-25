#include "sprintf.h"
#include <string.h>
#include "globals.h"
#include "mem/malloc.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define isdigit(c) (c >= '0' && c <= '9')
int sprintf(char *str, const char *fmt, ...)
{
    va_list ap;
    int i;
    va_start(ap, fmt);
    i = vsprintf(str, fmt, ap);
    va_end(ap);
    return i;
}

int snprintf(char *str, size_t size, const char *fmt, ...)
{
    va_list ap;
    int i;
    va_start(ap, fmt);
    i = vsnprintf(str, size, fmt, ap);
    va_end(ap);
    return i;
}

int vsprintf(char *str, const char *fmt, va_list ap)
{
    return vsnprintf(str, SIZE_MAX, fmt, ap);
}

int asprintf(char **strp, const char *fmt, ...)
{
    va_list ap;
    int i;
    va_start(ap, fmt);
    i = vasprintf(strp, fmt, ap);
    va_end(ap);
    return i;
}

int vasprintf(char **strp, const char *fmt, va_list ap)
{
    va_list ap2;
    int i;
    va_copy(ap2, ap);
    i = vsnprintf(NULL, 0, fmt, ap2);
    *strp = malloc(i+1);
    vsprintf(*strp, fmt, ap);
    return i;
}

#define FLAG_ALT 0x1
#define FLAG_ZERO 0x2
#define FLAG_LEFT 0x4
#define FLAG_SPACE 0x8
#define FLAG_SIGN 0x10
#define LENGTH_DEFAULT 0
#define LENGTH_CHAR 1
#define LENGTH_SHORT 2
#define LENGTH_LONG 3
#define LENGTH_LONGLONG 4
#define LENGTH_LONGDOUBLE 5
#define LENGTH_INTMAX 6
#define LENGTH_SIZE 7
#define LENGTH_PTRDIFF 8
struct printf_status {
    size_t n;
    uint32_t flags;
    int width;
    int precision;
    int length;
};

static void print_char(char **buf, size_t size, struct printf_status *status, int x)
{
    unsigned char c = (unsigned char)x;
    while (status->width > 1 && !(status->flags & FLAG_LEFT)) {
        if (status->n < size) *(*buf)++ = ' ';
        status->n++;
        status->width--;
    }
    if (status->n < size) *(*buf)++ = c;
    status->n++;
    while (status->width > 1 && status->flags & FLAG_LEFT) {
        if (status->n < size) *(*buf)++ = ' ';
        status->n++;
        status->width--;
    }
}

static void print_str(char **buf, size_t size, struct printf_status *status, const char *s)
{
    size_t l = strlen(s);
    if (status->precision >= 0 && status->precision < (int)l) l = status->precision;
    while (status->width > (int)l && !(status->flags & FLAG_LEFT)) {
        if (status->n < size) *(*buf)++ = ' ';
        status->n++;
        status->width--;
    }
    while (*s != '\0' && status->precision != 0) {
        if (status->n < size) *(*buf)++ = *s;
        s++;
        status->precision--;
        status->n++;
    }
    while (status->width > (int)l && status->flags & FLAG_LEFT) {
        if (status->n < size) *(*buf)++ = ' ';
        status->n++;
        status->width--;
    }
}

static void print_signed(char **buf, size_t size, struct printf_status *status, intmax_t x)
{
    char s[3*sizeof(intmax_t)]; // decimal takes at most 3 chars/byte
    bool negative = false;
    if (x < 0) {
        negative = true;
        x = -x;
    }
    int len=0;
    while (x>0) {
        s[len++] = x % 10 + '0';
        x /= 10;
    }

    // pad to precision
    if (status->precision < 0) status->precision = 1;
    while (len < status->precision) {
        s[len++] = '0';
    }

    char sign = 0;
    if (negative) sign = '-';
    else if (status->flags & FLAG_SIGN) sign = '+';
    else if (status->flags & FLAG_SPACE) sign = ' ';
    if (sign != 0) {
        s[len++] = sign;
    }

    // left pad to width
    char pad = (status -> flags & FLAG_ZERO) ? '0' : ' ';
    while (status->width > len && !(status->flags & FLAG_LEFT)) {
        if (status->n < size) *(*buf)++ = pad;
        status->n++;
        status->width--;
    }

    // print the number
    for (int i=len-1; i >= 0; i--) {
        if (status->n < size) *(*buf)++ = s[i];
        status->n++;
    }

    // right pad to width
    while (status->width > len && status->flags & FLAG_LEFT) {
        if (status->n < size) *(*buf)++ = pad;
        status->n++;
        status->width--;
    }
}

static void print_unsigned(char **buf, size_t size, struct printf_status *status, char type, uintmax_t x)
{
    char s[3*sizeof(uintmax_t)]; // octal takes at most 3 chars/byte
    int base = 10;
    if (type == 'o') base = 8;
    if (type == 'x') base = 16;
    if (type == 'X') base = 16;
    char *digits = "0123456789abcdef";
    if (type == 'X') digits = "0123456789ABCDEF";

    int len = 0;
    while (x>0) {
        s[len++] = digits[x % base];
        x /= base;
    }

    // pad to precision
    if (status->precision < 0) status->precision = 1;
    while (len < status->precision) {
        s[len++] = '0';
    }

    // add prefix for ALT
    if (status->flags & FLAG_ALT && type == 'o') {
        if (s[len-1] != '0') s[len++] = '0';
    } else if (status->flags & FLAG_ALT && type == 'x') {
        s[len++] = 'x';
        s[len++] = '0';
    } else if (status->flags & FLAG_ALT && type == 'X') {
        s[len++] = 'X';
        s[len++] = '0';
    }

    // left pad to width
    char pad = (status -> flags & FLAG_ZERO) ? '0' : ' ';
    while (status->width > len && !(status->flags & FLAG_LEFT)) {
        if (status->n < size) *(*buf)++ = pad;
        status->n++;
        status->width--;
    }

    // print the number
    for (int i=len-1; i >= 0; i--) {
        if (status->n < size) *(*buf)++ = s[i];
        status->n++;
    }

    // right pad to width
    while (status->width > len && status->flags & FLAG_LEFT) {
        if (status->n < size) *(*buf)++ = pad;
        status->n++;
        status->width--;
    }
}

static void store_length(struct printf_status *status, void *x)
{
    switch (status->length) {
    case LENGTH_CHAR:
        *((char *)x) = status->n;
        break;
    case LENGTH_SHORT:
        *((short *)x) = status->n;
        break;
    case LENGTH_LONG:
        *((long *)x) = status->n;
        break;
    case LENGTH_LONGLONG:
        *((long long *)x) = status->n;
        break;
    default:
        *((int *)x) = status->n;
        break;
    }
}

int vsnprintf(char *str, size_t size, const char *fmt, va_list ap)
{
    struct printf_status status;
    status.n = 0;
    for (char *buf = str; *fmt != '\0';) {
        status.flags = 0;
        status.width = -1;
        status.precision = -1;
        status.length = 0;
        if (*fmt != '%') {
            if (status.n < size) *buf++ = *fmt;
            fmt++;
            status.n++;
            continue;
        }
        fmt++;
        // get flags
        while (strchr("#0- +", *fmt) != NULL) {
            switch (*fmt) {
            case '#':
                status.flags |= FLAG_ALT;
                break;
            case '0':
                status.flags |= FLAG_ZERO;
                break;
            case '-':
                status.flags |= FLAG_LEFT;
                break;
            case ' ':
                status.flags |= FLAG_SPACE;
                break;
            case '+':
                status.flags |= FLAG_SIGN;
                break;
            }
            fmt++;
        }
        // get field width
        if (*fmt == '*') {
            status.width = va_arg(ap, int);
            if (status.width < 0) {
                status.flags |= FLAG_LEFT;
                status.width = -status.width;
            }
            fmt++;
        } else if (isdigit(*fmt)) {
            status.width = 0;
            while (isdigit(*fmt)) {
                status.width *= 10;
                status.width += *fmt++-'0';
            }
        }
        // get precision
        if (*fmt == '.') {
            fmt++;
            if (*fmt == '*') {
                status.precision = va_arg(ap, int);
                fmt++;
            } else {
                status.precision = 0;
                while (isdigit(*fmt)) {
                    status.precision *= 10;
                    status.precision += *fmt++-'0';
                }
            }
        }
        // get length modifier
        switch (*fmt) {
        case 'h':
            if (fmt[1] == 'h') {
                status.length = LENGTH_CHAR;
                fmt+=2;
            } else {
                status.length = LENGTH_SHORT;
                fmt++;
            }
            break;
        case 'l':
            if (fmt[1] == 'l') {
                status.length = LENGTH_LONGLONG;
                fmt+=2;
            } else {
                status.length = LENGTH_LONG;
                fmt++;
            }
            break;
        case 'L':
            status.length = LENGTH_LONGDOUBLE;
            fmt++;
            break;
        case 'j':
            status.length = LENGTH_INTMAX;
            fmt++;
            break;
        case 'z':
            status.length = LENGTH_SIZE;
            fmt++;
            break;
        case 't':
            status.length = LENGTH_PTRDIFF;
            fmt++;
            break;
        }
        // get type
        switch (*fmt) {
        case 'd':
        case 'i':
            switch (status.length) {
            case LENGTH_CHAR:
                print_signed(&buf, size, &status, va_arg(ap, int));
                break;
            case LENGTH_SHORT:
                print_signed(&buf, size, &status, va_arg(ap, int));
                break;
            case LENGTH_LONG:
                print_signed(&buf, size, &status, va_arg(ap, long));
                break;
            case LENGTH_LONGLONG:
                print_signed(&buf, size, &status, va_arg(ap, long long));
                break;
            case LENGTH_INTMAX:
                print_signed(&buf, size, &status, va_arg(ap, intmax_t));
                break;
            case LENGTH_SIZE:
                print_signed(&buf, size, &status, va_arg(ap, size_t));
                break;
            case LENGTH_PTRDIFF:
                print_signed(&buf, size, &status, va_arg(ap, ptrdiff_t));
                break;
            default:
                print_signed(&buf, size, &status, va_arg(ap, int));
                break;
            }
            break;
        case 'o':
        case 'u':
        case 'x':
        case 'X':
            switch (status.length) {
            case LENGTH_CHAR:
                print_unsigned(&buf, size, &status, *fmt, va_arg(ap, unsigned int));
                break;
            case LENGTH_SHORT:
                print_unsigned(&buf, size, &status, *fmt, va_arg(ap, unsigned int));
                break;
            case LENGTH_LONG:
                print_unsigned(&buf, size, &status, *fmt, va_arg(ap, unsigned long));
                break;
            case LENGTH_LONGLONG:
                print_unsigned(&buf, size, &status, *fmt, va_arg(ap, unsigned long long));
                break;
            case LENGTH_INTMAX:
                print_unsigned(&buf, size, &status, *fmt, va_arg(ap, uintmax_t));
                break;
            case LENGTH_SIZE:
                print_unsigned(&buf, size, &status, *fmt, va_arg(ap, size_t));
                break;
            case LENGTH_PTRDIFF:
                print_unsigned(&buf, size, &status, *fmt, va_arg(ap, ptrdiff_t));
                break;
            default:
                print_unsigned(&buf, size, &status, *fmt, va_arg(ap, unsigned int));
                break;
            }
            break;
        case 'e':
        case 'E':
            // scientific
            break;
        case 'f':
        case 'F':
            // double
            break;
        case 'g':
        case 'G':
            // optional scientific
            break;
        case 'a':
        case 'A':
            // hex float
            break;
        case 'c':
            print_char(&buf, size, &status, va_arg(ap, int));
            break;
        case 's':
            print_str(&buf, size, &status, va_arg(ap, char *));
            break;
        case 'p':
            status.flags |= FLAG_ALT;
            print_unsigned(&buf, size, &status, 'x', (uint32_t)va_arg(ap, void *));
            break;
        case 'n':
            store_length(&status, va_arg(ap, void *));
            break;
        case '%':
            if (status.n < size) *buf++ = '%';
            status.n++;
            break;
        }
        fmt++;
    }
    if (size == 0) {
        // do nothing
    } else if (status.n >= size) {
        str[size-1] = '\0';
    } else {
        str[status.n] = '\0';
    }
    return status.n;
}
