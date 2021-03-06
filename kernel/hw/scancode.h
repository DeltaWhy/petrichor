// XT scancodes
#define SCAN_LSHIFT 0x2a
#define SCAN_RSHIFT 0x36
#define SCAN_CAPSLOCK 0x3a
#define SCAN_BACKSPACE 0x0e
#define SCAN_E_PGUP 0x49
#define SCAN_E_PGDN 0x51

static char scan_shift1[] = {
    /* 0x00 */ 0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t',
    /* 0x10 */ 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0 /* ctrl */, 'A', 'S',
    /* 0x20 */ 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0 /* shift */, '|', 'Z', 'X', 'C', 'V',
    /* 0x30 */ 'B', 'N', 'M', '<', '>', '?', 0 /* rshift */, '*', 0 /* alt */, ' ', 0 /* caps */, 0, 0, 0, 0, 0, /* f1-f5 */
    /* 0x40 */ 0, 0, 0, 0, 0 /* f6-f10 */, 0 /* num */, 0 /* scroll */, '7', '8', '9', '-', '4', '5', '6', '+', '1',
    /* 0x50 */ '2', '3', '0', '.', 0, 0, 0 /* none */, 0, 0 /* f11-f12 */, 0, 0, 0, 0, 0, 0, 0, /* none */
    /* 0x60 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* none */
    /* 0x70 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 /* none */
};

static char scan_normal1[] = {
    /* 0x00 */ 0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
    /* 0x10 */ 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0 /* ctrl */, 'a', 's',
    /* 0x20 */ 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0 /* shift */, '\\', 'z', 'x', 'c', 'v',
    /* 0x30 */ 'b', 'n', 'm', ',', '.', '/', 0 /* rshift */, '*', 0 /* alt */, ' ', 0 /* caps */, 0, 0, 0, 0, 0, /* f1-f5 */
    /* 0x40 */ 0, 0, 0, 0, 0 /* f6-f10 */, 0 /* num */, 0 /* scroll */, '7', '8', '9', '-', '4', '5', '6', '+', '1',
    /* 0x50 */ '2', '3', '0', '.', 0, 0, 0 /* none */, 0, 0 /* f11-f12 */, 0, 0, 0, 0, 0, 0, 0, /* none */
    /* 0x60 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* none */
    /* 0x70 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 /* none */
};

static char scan_caps1[] = {
    /* 0x00 */ 0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
    /* 0x10 */ 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n', 0 /* ctrl */, 'A', 'S',
    /* 0x20 */ 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`', 0 /* shift */, '\\', 'Z', 'X', 'C', 'V',
    /* 0x30 */ 'B', 'N', 'M', ',', '.', '/', 0 /* rshift */, '*', 0 /* alt */, ' ', 0 /* caps */, 0, 0, 0, 0, 0, /* f1-f5 */
    /* 0x40 */ 0, 0, 0, 0, 0 /* f6-f10 */, 0 /* num */, 0 /* scroll */, '7', '8', '9', '-', '4', '5', '6', '+', '1',
    /* 0x50 */ '2', '3', '0', '.', 0, 0, 0 /* none */, 0, 0 /* f11-f12 */, 0, 0, 0, 0, 0, 0, 0, /* none */
    /* 0x60 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* none */
    /* 0x70 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 /* none */
};

static char scan_shift_caps1[] = {
    /* 0x00 */ 0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t',
    /* 0x10 */ 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}', '\n', 0 /* ctrl */, 'a', 's',
    /* 0x20 */ 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', '"', '~', 0 /* shift */, '|', 'z', 'x', 'c', 'v',
    /* 0x30 */ 'b', 'n', 'm', '<', '>', '?', 0 /* rshift */, '*', 0 /* alt */, ' ', 0 /* caps */, 0, 0, 0, 0, 0, /* f1-f5 */
    /* 0x40 */ 0, 0, 0, 0, 0 /* f6-f10 */, 0 /* num */, 0 /* scroll */, '7', '8', '9', '-', '4', '5', '6', '+', '1',
    /* 0x50 */ '2', '3', '0', '.', 0, 0, 0 /* none */, 0, 0 /* f11-f12 */, 0, 0, 0, 0, 0, 0, 0, /* none */
    /* 0x60 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* none */
    /* 0x70 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 /* none */
};




static char scan_normal_dvorak[] = {
    /* 0x00 */ 0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '[', ']', '\b', '\t',
    /* 0x10 */ '\'', ',', '.', 'p', 'y', 'f', 'g', 'c', 'r', 'l', '/', '=', '\n', 0 /* ctrl */, 'a', 'o',
    /* 0x20 */ 'e', 'u', 'i', 'd', 'h', 't', 'n', 's', '-', '`', 0 /* shift */, '\\', ';', 'q', 'j', 'k',
    /* 0x30 */ 'x', 'b', 'm', 'w', 'v', 'z', 0 /* rshift */, '*', 0 /* alt */, ' ', 0 /* caps */, 0, 0, 0, 0, 0, /* f1-f5 */
    /* 0x40 */ 0, 0, 0, 0, 0 /* f6-f10 */, 0 /* num */, 0 /* scroll */, '7', '8', '9', '-', '4', '5', '6', '+', '1',
    /* 0x50 */ '2', '3', '0', '.', 0, 0, 0 /* none */, 0, 0 /* f11-f12 */, 0, 0, 0, 0, 0, 0, 0, /* none */
    /* 0x60 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* none */
    /* 0x70 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 /* none */
};



static char scan_caps_dvorak[] = {
    /* 0x00 */ 0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '[', ']', '\b', '\t',
    /* 0x10 */ '\'', ',', '.', 'P', 'Y', 'F', 'G', 'C', 'R', 'L', '/', '=', '\n', 0 /* ctrl */, 'A', 'O',
    /* 0x20 */ 'E', 'U', 'I', 'D', 'H', 'T', 'N', 'S', '-', '`', 0 /* shift */, '\\', ';', 'Q', 'J', 'K',
    /* 0x30 */ 'X', 'B', 'M', 'W', 'V', 'Z', 0 /* rshift */, '*', 0 /* alt */, ' ', 0 /* caps */, 0, 0, 0, 0, 0, /* f1-f5 */
    /* 0x40 */ 0, 0, 0, 0, 0 /* f6-f10 */, 0 /* num */, 0 /* scroll */, '7', '8', '9', '-', '4', '5', '6', '+', '1',
    /* 0x50 */ '2', '3', '0', '.', 0, 0, 0 /* none */, 0, 0 /* f11-f12 */, 0, 0, 0, 0, 0, 0, 0, /* none */
    /* 0x60 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* none */
    /* 0x70 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 /* none */
};



static char scan_shift_dvorak[] = {
    /* 0x00 */ 0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '{', '}', '\b', '\t',
    /* 0x10 */ '"', '<', '>', 'P', 'Y', 'F', 'G', 'C', 'R', 'L', '?', '+', '\n', 0 /* ctrl */, 'A', 'O',
    /* 0x20 */ 'E', 'U', 'I', 'D', 'H', 'T', 'N', 'S', '_', '~', 0 /* shift */, '|', ':', 'Q', 'J', 'K',
    /* 0x30 */ 'X', 'B', 'M', 'W', 'V', 'Z', 0 /* rshift */, '*', 0 /* alt */, ' ', 0 /* caps */, 0, 0, 0, 0, 0, /* f1-f5 */
    /* 0x40 */ 0, 0, 0, 0, 0 /* f6-f10 */, 0 /* num */, 0 /* scroll */, '7', '8', '9', '-', '4', '5', '6', '+', '1',
    /* 0x50 */ '2', '3', '0', '.', 0, 0, 0 /* none */, 0, 0 /* f11-f12 */, 0, 0, 0, 0, 0, 0, 0, /* none */
    /* 0x60 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* none */
    /* 0x70 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 /* none */
};


static char scan_shift_caps_dvorak[] = {
    /* 0x00 */ 0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '{', '}', '\b', '\t',
    /* 0x10 */ '"', '<', '>', 'p', 'y', 'f', 'g', 'c', 'r', 'l', '?', '+', '\n', 0 /* ctrl */, 'a', 'o',
    /* 0x20 */ 'e', 'u', 'i', 'd', 'h', 't', 'n', 's', '_', '~', 0 /* shift */, '|', ':', 'q', 'j', 'k',
    /* 0x30 */ 'x', 'b', 'm', 'w', 'v', 'z', 0 /* rshift */, '*', 0 /* alt */, ' ', 0 /* caps */, 0, 0, 0, 0, 0, /* f1-f5 */
    /* 0x40 */ 0, 0, 0, 0, 0 /* f6-f10 */, 0 /* num */, 0 /* scroll */, '7', '8', '9', '-', '4', '5', '6', '+', '1',
    /* 0x50 */ '2', '3', '0', '.', 0, 0, 0 /* none */, 0, 0 /* f11-f12 */, 0, 0, 0, 0, 0, 0, 0, /* none */
    /* 0x60 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* none */
    /* 0x70 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 /* none */
};




