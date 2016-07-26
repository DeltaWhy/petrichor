#include <string.h>
#include "sprintf.h"
#include "globals.h"
#include "hw/keyboard.h"
#include "hw/port.h"
#include "hw/screen.h"
#include "cbuf.h"
#include "hw/scancode.h"
int led_stat=0;

static cbuf key_buf;
static cbuf line_buf;
static kbd_info keyboard;
char *scan_normal;
char *scan_shift;
char *scan_caps;
char *scan_shift_caps;

void init_keyboard()
{
    // I have no idea if this is correct
    outportb(KBD_CONTROLLER_REG, KBD_CTL_CMD_READ_CMD);
    char cmd = inportb(KBD_CONTROLLER_REG);

    printf("keyboard command byte: %#x\n", cmd);
    outportb(KBD_CONTROLLER_REG, KBD_CTL_CMD_WRITE_CMD);
    outportb(KBD_CONTROLLER_REG, cmd | KBD_CMD_KEYBOARD_INTERRUPT);

    cbuf_new(&key_buf);
    cbuf_new(&line_buf);
    keyboard.mode = 0;
    keyboard.held = 0;

    load_keys("standard");
}

bool load_keys (char *scancode)
{
    if (strcmp(scancode, "standard")==0) {
        scan_shift = scan_shift1;
        scan_normal = scan_normal1;

        scan_caps = scan_caps1;
        scan_shift_caps = scan_shift_caps1;
        return true;
    } else if (strcmp(scancode, "dvorak")==0) {
        scan_normal = scan_normal_dvorak;
        scan_shift = scan_shift_dvorak;
        scan_caps = scan_caps_dvorak;
        scan_shift_caps = scan_shift_caps_dvorak;
        return true;
    } else {
        return false;
    }
}

void keyboard_irq_handler()
{
    // put scancode in buffer
    while (!(inportb(KBD_CONTROLLER_REG)&KBD_STATS_OUT_BUF));
    int scancode = inportb(0x60);

    // 0xE0 is the first byte of a two-byte scancode
    if (scancode == 0xE0) {
        keyboard.state_extended = true;
        goto out;
    }
    if (keyboard.state_extended) {
        char printbuf[256];
        snprintf(printbuf, 256, "ext keycode: %02x\n", scancode);
        bochslog(printbuf);
        keyboard.state_extended = false;
        if (scancode & 0x80 && keyboard.held & (KBD_HELD_LSHIFT | KBD_HELD_RSHIFT)) {
            if ((scancode & ~(0x80)) == SCAN_E_PGUP) screen_scroll_up();
            else if ((scancode & ~(0x80)) == SCAN_E_PGDN) screen_scroll_down();
        }
        goto out;
    }

    if (keyboard.mode & KBD_INFO_MODE_RAW) {
        cbuf_push(&key_buf, scancode);
    }
    // handle special keys
    switch (scancode & ~(0x80)) {
    case SCAN_LSHIFT:
        if (scancode & 0x80) {
            keyboard.held &= ~(KBD_HELD_LSHIFT);
        } else {
            keyboard.held |= KBD_HELD_LSHIFT;
        }
        break;
    case SCAN_RSHIFT:
        if (scancode & 0x80) {
            keyboard.held &= ~(KBD_HELD_RSHIFT);
        } else {
            keyboard.held |= KBD_HELD_RSHIFT;
        }
        break;
    case SCAN_CAPSLOCK:
        if (!(scancode & 0x80)) {
            keyboard.held ^= KBD_HELD_CAPSLOCK;
            if (keyboard.held & KBD_HELD_CAPSLOCK) {
                led_stat |= KBD_LED_CAPS;
            } else {
                led_stat &= ~(KBD_LED_CAPS);
            }
            klights();
        }
        break;
    case SCAN_BACKSPACE:
        if (keyboard.mode & KBD_INFO_MODE_LINE && !(scancode & 0x80)) {
            if (cbuf_unpush(&line_buf)) screen_backspace();
        }
        break;
    }
    if (!(scancode & 0x80)) {
        char c = scancode_to_ascii(scancode);
        // characters below \t and above 127 are non-printing
        if (c >= '\t' && keyboard.mode & KBD_INFO_MODE_ECHO) {
            kputc(c, 0x07);
        }
        if (c && !(keyboard.mode & (KBD_INFO_MODE_RAW | KBD_INFO_MODE_LINE))) {
            // character mode
            cbuf_push(&key_buf, c);
        } else if (c >= '\t' && keyboard.mode & KBD_INFO_MODE_LINE) {
            if (c == '\n') {
                while (!cbuf_empty(&line_buf)) {
                    cbuf_push(&key_buf, cbuf_pop(&line_buf));
                }
                cbuf_push(&key_buf, '\n');
            } else {
                cbuf_push(&line_buf, c);
            }
        }
    }

    int i;
out:
    // reset the keyboard
    i = inportb(0x61);	//WEEP WEEP! MAGIC NUMBERS!
    outportb(0x61, i|0x80);
    outportb(0x61, i);
}



void klights()
{
    outportb(KBD_ENCODER_REG, KBD_ENC_CMD_SET_LED);
    while (inportb(KBD_CONTROLLER_REG) & KBD_STATS_IN_BUF);
    outportb(KBD_ENCODER_REG, led_stat);
    while (inportb(KBD_CONTROLLER_REG) & KBD_STATS_IN_BUF);
}

void kbd_set_mode(uint32_t mode)
{
    // if switching raw/cooked mode, flush the buffer
    if ((mode & KBD_INFO_MODE_RAW) ^ (keyboard.mode & KBD_INFO_MODE_RAW)) cbuf_flush(&key_buf);
    // if switching out of line mode, consume the line buffer
    if ((keyboard.mode & KBD_INFO_MODE_LINE) && !(mode & KBD_INFO_MODE_LINE)) {
        while (!cbuf_empty(&line_buf)) {
            cbuf_push(&key_buf, cbuf_pop(&line_buf));
        }
    }
    keyboard.mode = mode;
}

char scancode_to_ascii(int scancode)
{

    if (keyboard.held & (KBD_HELD_LSHIFT | KBD_HELD_RSHIFT) &&
            keyboard.held & KBD_HELD_CAPSLOCK) {
        return scan_shift_caps[scancode];
    } else if (keyboard.held & (KBD_HELD_LSHIFT | KBD_HELD_RSHIFT)) {
        return scan_shift[scancode];
    } else if (keyboard.held & KBD_HELD_CAPSLOCK) {
        return scan_caps[scancode];
    } else {
        return scan_normal[scancode];
    }
    return 0;
}

char getchar()
{
    while (cbuf_empty(&key_buf)) wait();
    return cbuf_pop(&key_buf);
}

char *kgets(char *buf)
{
    char *p = buf;
    char c;
    while ((c = getchar()) != '\n') {
        *p = c;
        p++;
    }
    *p = '\0';
    return buf;
}
