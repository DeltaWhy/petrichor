#ifndef RBT_H
#define RBT_H
// Red-Black Treemap
typedef enum { RBT_BLACK, RBT_RED } rbt_color;
struct rbt_node;
typedef struct rbt_node rbt_node;

typedef struct rbt_node {
	rbt_color color;
	char *key;
	void *value;
	rbt_node *parent;
	rbt_node *left;
	rbt_node *right;
} rbt_node;

typedef struct rbt {
	rbt_node *root;
} rbt;

void rbt_init(rbt *);
void rbt_cleanup(rbt *);
void * rbt_get(rbt *, char *);
void rbt_put(rbt *, char *, void *);
void * rbt_remove(rbt *, char *);

rbt_node * rbt_first(rbt *);
rbt_node * rbt_next(rbt_node *);

#define rbt_each(tree, p) \
	for (p = rbt_first(tree); p != NULL; p = rbt_next(p))
	
#endif
