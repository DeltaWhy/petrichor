#include <stdio.h>
#include <CUnit/CUnit.h>
#include "foo.h"

int suite_init_foo(void) {
	puts("suite_init_foo");
	return 0;
}

int suite_cleanup_foo(void) {
	puts("suite_cleanup_foo");
	return 0;
}

void test_init_foo(void) {
	puts("test_init_foo");
}

void test_cleanup_foo(void) {
	puts("test_cleanup_foo");
}

void test_foo(void) {
	puts("\thello foo");
	foo();
}

void test_foo_bar(void) {
	puts("\thello foo bar");
}

CU_SuiteInfo foo_suite = {"foo", suite_init_foo, suite_cleanup_foo, test_init_foo, test_cleanup_foo, (CU_TestInfo[]){
	{ "test_foo", test_foo },
	{ "test_foo_bar", test_foo_bar },
	CU_TEST_INFO_NULL
}};
