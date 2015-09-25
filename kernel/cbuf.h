#ifndef CBUF_H
#define CBUF_H

#define CBUF_SIZE 1024
typedef struct cbuf {
    int size;
    char data[CBUF_SIZE];
    int start;
    int end;
} cbuf;

void cbuf_new(cbuf *buf);

void cbuf_push(cbuf *buf, char c);
char cbuf_pop(cbuf *buf);
char cbuf_peek(cbuf *buf);
void cbuf_unpop(cbuf *buf, char c);
char cbuf_unpush(cbuf *buf);

int cbuf_empty(cbuf *buf);
int cbuf_read(cbuf *buf, char *out, int length);
int cbuf_write(cbuf *buf, char *data, int length);
void cbuf_flush(cbuf *buf);
#endif
