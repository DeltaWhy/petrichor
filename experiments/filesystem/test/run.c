#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdio.h>
CU_SuiteInfo foo_suite;

int main(int argc, char **argv) {
	if (CU_initialize_registry() != CUE_SUCCESS) {
		fprintf(stderr, "%s\n", CU_get_error_msg());
		return CU_get_error();
	}

	CU_register_suites((CU_SuiteInfo[]){foo_suite, CU_SUITE_INFO_NULL});

	CU_basic_set_mode(CU_BRM_VERBOSE);
	if (CU_basic_run_tests() != CUE_SUCCESS) {
		fprintf(stderr, "%s\n", CU_get_error_msg());
		CU_cleanup_registry();
		return CU_get_error();
	}

	unsigned int res = CU_get_number_of_failures();
	CU_cleanup_registry();
	return res;
}
