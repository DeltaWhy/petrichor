#include <CUnit/CUnit.h>
#include "llist.h"

void test_llist(void) {
	// some data
	int one = 1;
	int two = 2;
	int three = 3;

	// create list
	llist *l = llist_new();
	CU_ASSERT(l != NULL);

	// test empty list
	CU_ASSERT_EQUAL(llist_len(l), 0);
	CU_ASSERT_EQUAL(llist_get(l, 0), NULL);
	CU_ASSERT_EQUAL(llist_put(l, 0, &one), false);

	// insert one element
	CU_ASSERT_EQUAL(llist_insert(l, 0, &one), true);
	CU_ASSERT_EQUAL(llist_get(l, 0), &one);
	CU_ASSERT_EQUAL(llist_get(l, 1), NULL);
	CU_ASSERT_EQUAL(llist_len(l), 1);
	
	// replace element
	CU_ASSERT_EQUAL(llist_put(l, 0, &two), true);
	CU_ASSERT_EQUAL(llist_get(l, 0), &two);
	CU_ASSERT_EQUAL(llist_len(l), 1);

	// add more elements
	CU_ASSERT_EQUAL(llist_insert(l, 0, &one), true);
	CU_ASSERT_EQUAL(llist_insert(l, 2, &three), true);
	CU_ASSERT_EQUAL(llist_get(l, 0), &one);
	CU_ASSERT_EQUAL(llist_get(l, 1), &two);
	CU_ASSERT_EQUAL(llist_get(l, 2), &three);
	CU_ASSERT_EQUAL(llist_len(l), 3);

	// remove element
	CU_ASSERT_EQUAL(llist_remove(l, 1), &two);
	CU_ASSERT_EQUAL(llist_get(l, 1), &three);
	CU_ASSERT_EQUAL(llist_len(l), 2);
	CU_ASSERT_EQUAL(llist_remove(l, 2), NULL);
	CU_ASSERT_EQUAL(llist_len(l), 2);

	// clean up
	llist_del(l);
}

CU_SuiteInfo llist_suite = {"llist", NULL, NULL, NULL, NULL, (CU_TestInfo[]){
	{ "test_llist", test_llist },
	CU_TEST_INFO_NULL
}};
