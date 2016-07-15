#include <CUnit/CUnit.h>
#include <stdlib.h>
#include <string.h>
#include "rbt.h"

void test_rbt(void) {
	// some data
	int one = 1;
	int two = 2;
	int three = 3;
	int four = 4;

	// create tree
	rbt tree;
	rbt_init(&tree);

	rbt_node *p;
	// tree is empty
	rbt_each(&tree, p) {
		CU_FAIL("tree not empty");
	}

	// insert some data
	rbt_put(&tree, "m", &one);
	rbt_put(&tree, "b", &two);
	rbt_put(&tree, "a", &three);
	rbt_put(&tree, "f", &four);
	rbt_put(&tree, "d", &one);
	rbt_put(&tree, "c", &one);
	rbt_put(&tree, "e", &two);
	rbt_put(&tree, "o", &two);
	rbt_put(&tree, "n", &three);
	rbt_put(&tree, "t", &three);
	rbt_put(&tree, "r", &four);
	rbt_put(&tree, "s", &four);
	rbt_put(&tree, "w", &one);
	rbt_put(&tree, "u", &two);
	rbt_put(&tree, "v", &three);
	rbt_put(&tree, "x", &four);

	// check ordering
	char keys[17] = "";
	void *values[16];
	int i = 0;
	rbt_each(&tree, p) {
		CU_ASSERT_FATAL(i < 16);
		strcat(keys, p->key);
		values[i] = p->value;
		i++;
	}
	CU_ASSERT(strcmp("abcdefmnorstuvwx", keys) == 0);
	CU_ASSERT_EQUAL(values[0], &three);
	CU_ASSERT_EQUAL(values[1], &two);
	CU_ASSERT_EQUAL(values[2], &one);
	CU_ASSERT_EQUAL(values[3], &one);
	CU_ASSERT_EQUAL(values[4], &two);
	CU_ASSERT_EQUAL(values[5], &four);
	CU_ASSERT_EQUAL(values[6], &one);
	CU_ASSERT_EQUAL(values[7], &three);
	CU_ASSERT_EQUAL(values[8], &two);
	CU_ASSERT_EQUAL(values[9], &four);
	CU_ASSERT_EQUAL(values[10], &four);
	CU_ASSERT_EQUAL(values[11], &three);
	CU_ASSERT_EQUAL(values[12], &two);
	CU_ASSERT_EQUAL(values[13], &three);
	CU_ASSERT_EQUAL(values[14], &one);
	CU_ASSERT_EQUAL(values[15], &four);
	
	// test getting
	CU_ASSERT_EQUAL(rbt_get(&tree, "f"), &four);
	CU_ASSERT_EQUAL(rbt_get(&tree, "v"), &three);

	// test replacement
	rbt_put(&tree, "f", &one);
	CU_ASSERT_EQUAL(rbt_get(&tree, "f"), &one);
	CU_ASSERT_EQUAL(rbt_get(&tree, "v"), &three);

	rbt_cleanup(&tree);
}

CU_SuiteInfo rbt_suite = {"rbt", NULL, NULL, NULL, NULL, (CU_TestInfo[]){
	{ "test_rbt", test_rbt },
	CU_TEST_INFO_NULL
}};
