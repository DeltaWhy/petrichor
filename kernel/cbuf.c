#include "cbuf.h"

void cbuf_new(cbuf *buf)
{
    buf->size = CBUF_SIZE;
    buf->start = 0;
    buf->end = 0;
}


void cbuf_push(cbuf *buf, char c)
{
    buf->data[buf->end] = c;
    buf->end = (buf->end+1) % buf->size;
    if (buf->start == buf->end)
        buf->start = (buf->start+1) % buf->size;
}

char cbuf_pop(cbuf *buf)
{
    if (cbuf_empty(buf)) return '\0';
    char c = buf->data[buf->start];
    buf->start = (buf->start+1) % buf->size;
    return c;
}

char cbuf_peek(cbuf *buf)
{
    if (cbuf_empty(buf)) return '\0';
    return buf->data[buf->start];
}

void cbuf_unpop(cbuf *buf, char c)
{
    int i = (buf->start+buf->size-1) % buf->size;
    if (i == buf->end) return;
    buf->start = i;
    buf->data[buf->start] = c;
}

char cbuf_unpush(cbuf *buf)
{
    if (cbuf_empty(buf)) return '\0';
    buf->end = (buf->end+buf->size-1) % buf->size;
    return buf->data[buf->end];
}

int cbuf_empty(cbuf *buf)
{
    return buf->start == buf->end;
}

int cbuf_read(cbuf *buf, char *out, int length)
{
    int total = 0;
    while (total < length && !cbuf_empty(buf)) {
        *out = cbuf_pop(buf);
        out++;
        total++;
    }
    return total;
}

int cbuf_write(cbuf *buf, char *data, int length)
{
    for (int i=0; i < length; i++) {
        cbuf_push(buf, *data);
        data++;
    }
    return length;
}

void cbuf_flush(cbuf *buf)
{
    buf->start = buf->end;
}
