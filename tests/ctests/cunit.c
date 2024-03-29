#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "utils.h"
#include "engine_pin.h"


int init_suite1(void)
{
    return 0;
}


int clean_suite1(void)
{
    return 0;
}


void test_strcatn(void)
{
    char *actual = NULL;
    char *s1 = "a";
    char *s2 = "b";
    char *s3 = "c";
    strcatn(&actual, s1, s2, s3, NULL);
    CU_ASSERT_STRING_EQUAL(actual, "abc");
}


int main()
{
    CU_pSuite pSuite = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* add a suite to the registry */
    pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    /* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
    if ((NULL == CU_add_test(pSuite, "test of strcatn()", test_strcatn)) || 0)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
