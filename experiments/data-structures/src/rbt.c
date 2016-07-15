#include <stdlib.h>
#include <string.h>
#include "rbt.h"

inline rbt_node * rbt_first(rbt *t) {
	rbt_node *p = t->root;
	while (p != NULL && p->left != NULL) {
		p = p->left;
	}
	return p;
}

inline rbt_node * rbt_next(rbt_node *n) {
	if (n->right != NULL) {
		rbt_node *p = n->right;
		while (p->left != NULL) p = p->left;
		return p;
	} else if (n->parent != NULL && n->parent->left == n) {
		return n->parent;
	} else if (n->parent != NULL && n->parent->right == n) {
		rbt_node *p = n->parent;
		while (p->parent) {
			if (p == p->parent->left) return p->parent;
			p = p->parent;
		}
		return NULL;
	} else {
		// TODO
		return NULL;
	}
}

static rbt_node * rbt_node_new(void) {
	rbt_node *node = malloc(sizeof(rbt_node));
	node->color = RBT_BLACK;
	node->key = NULL;
	node->value = NULL;
	node->parent = NULL;
	node->left = NULL;
	node->right = NULL;
	return node;
}

void rbt_init(rbt *t) {
	t->root = NULL;
}

void rbt_cleanup(rbt *t) {

}

void * rbt_get(rbt *t, char *k) {
	rbt_node *p = t->root;
	while (p != NULL) {
		int cmp = strcmp(k, p->key);
		if (cmp == 0) {
			return p->value;
		} else if (cmp < 0) {
			p = p->left;
		} else {
			p = p->right;
		}
	}
	return NULL;
}

void rbt_put(rbt *t, char *k, void *v) {
	if (t->root == NULL) {
		// easy case
		t->root = rbt_node_new();
		t->root->key = malloc(strlen(k)+1);
		strcpy(t->root->key, k);
		t->root->value = v;
		return;
	}
	rbt_node *p = t->root;
	while (p != NULL) {
		int cmp = strcmp(k, p->key);
		if (cmp == 0) {
			p->value = v;
			return;
		} else if (cmp < 0) {
			if (p->left == NULL) {
				// insert new node
				rbt_node *n = rbt_node_new();
				n->parent = p;
				n->key = malloc(strlen(k)+1);
				strcpy(n->key, k);
				n->value = v;
				p->left = n;
				return;
			}
			p = p->left;
		} else {
			if (p->right == NULL) {
				// insert new node
				rbt_node *n = rbt_node_new();
				n->parent = p;
				n->key = malloc(strlen(k)+1);
				strcpy(n->key, k);
				n->value = v;
				p->right = n;
				return;
			}
			p = p->right;
		}
	}
}

void * rbt_remove(rbt *t, char *k) {
	return NULL;
}
