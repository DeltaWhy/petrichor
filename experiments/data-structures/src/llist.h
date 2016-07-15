#ifndef LLIST_H
#define LLIST_H
#include <stdbool.h>
struct llist;
typedef struct llist llist;

llist * llist_new(void);
void llist_del(llist *);

int llist_len(llist *);
void * llist_get(llist *, int);
bool llist_put(llist *, int, void *);
bool llist_insert(llist *, int, void *);
void * llist_remove(llist *, int);
#endif
