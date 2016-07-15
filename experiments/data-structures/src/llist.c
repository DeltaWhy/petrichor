#include <stdlib.h>
#include "llist.h"
struct llist {
	void * data;
	llist * next;
};

llist * llist_new(void) {
	llist *l = malloc(sizeof(llist));
	l->data = NULL;
	l->next = NULL;
	return l;
}

void llist_del(llist *l) {
	l = l->next;
	while (l != NULL) {
		llist *n = l->next;
		free(l);
		l = n;
	}
}

int llist_len(llist *l) {
	l = l->next;
	int len = 0;
	while (l != NULL) {
		len++;
		l = l->next;
	}
	return len;
}

void * llist_get(llist *l, int i) {
	l = l->next;
	int n = -1;
	while (l != NULL) {
		n++;
		if (i == n) return l->data;
		l = l->next;
	}
	return NULL;
}

bool llist_put(llist *l, int i, void *data) {
	l = l->next;
	int n = -1;
	while (l != NULL) {
		n++;
		if (i == n) {
			l->data = data;
			return true;
		}
		l = l->next;
	}
	return false;
}

bool llist_insert(llist *l, int i, void *data) {
	int n = 0;
	while (n < i) {
		if (l->next == NULL) return false;
		l = l->next;
		n++;
	}
	llist *node = malloc(sizeof(llist));
	node->data = data;
	node->next = l->next;
	l->next = node;
	return true;
}

void * llist_remove(llist *l, int i) {
	int n = 0;
	while (n < i) {
		if (l->next == NULL) return false;
		l = l->next;
		n++;
	}
	if (l->next == NULL) return false;

	llist *node = l->next;
	void *data = node->data;
	l->next = node->next;
	free(node);
	return data;
}
