#include <stdarg.h>
#include <setjmp.h>
#include <stddef.h>
#include <cmocka.h>
#include <cjson/cJSON.h>

static void test(void **state)
{
    assert_int_equal(2, 2);
}

int main()
{
    cJSON *json = cJSON_Parse("{\"key1\":\"val1\"}");
    const struct CMUnitTest tests[] =
    {
        cmocka_unit_test(test),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
