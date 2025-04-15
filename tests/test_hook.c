/*
** EPITECH PROJECT, 2025
** test_hook.c
** File description:
** hook for the test
*/

#include <criterion/hooks.h>
#include <criterion/logging.h>
#include <stdio.h>

ReportHook(PRE_SUITE)(struct criterion_suite_set *test_suite)
{
    printf("\nActual test suite is \e[1;34m%s\e[0m\n"
        "Description of this suite :\n\e[32m%s\e[0m\n\n",
        test_suite->suite.name, test_suite->suite.data->description);
}

ReportHook(POST_SUITE)(struct criterion_suite_stats *suite_stat)
{
    double success_percent = -1;

    printf("\nResult of the test suite \e[1;34m%s\e[0m\n"
        "\tNumber of tests : \e[1;34m%li\e[0m\n"
        "\t\e[36mTests that passed :\e[0m \e[1;34m%li\e[0m\n"
        "\t\e[31mTests that failed :\e[0m \e[1;34m%li\e[0m\n"
        "\t\tOf these, tests that \e[31mcrashed\e[0m : \e[1;34m%li\e[0m\n"
        "\t\tOf these, tests that \e[31mfailed\e[0m : \e[1;34m%li\e[0m\n"
        "\t\tOf these, tests that were \e[33mskipped\e[0m : \e[1;34m%li\e[0m\n"
        "\tNumber of assert : \e[1;34m%li\e[0m\n"
        "\t\t\e[36mOf these, assert that passed :\e[0m  \e[1;34m%li\e[0m\n"
        "\t\t\e[31mOf these, tests that failed :\e[0m  \e[1;34m%li\e[0m\n"
        , suite_stat->suite->name, suite_stat->nb_tests,
        suite_stat->tests_passed,
        suite_stat->tests_skipped + suite_stat->tests_crashed + suite_stat->tests_failed,
        suite_stat->tests_crashed, suite_stat->tests_failed, suite_stat->tests_skipped,
        suite_stat->nb_asserts, suite_stat->asserts_passed, suite_stat->asserts_failed);
    if (suite_stat->nb_tests != 0)
        success_percent = ((double)suite_stat->tests_passed / suite_stat->nb_tests) * 100;
    if (success_percent < 0 || success_percent > 100)
        printf("Unable to compute an percentage of success\n");
    if (success_percent >= 0 && success_percent <= 70)
        printf("\e[31mSuccess rate of \e[1;34m%.2f%%\e[0m !\n"
        "You might want to check back your code and fix these issue !\e[0m\n", success_percent);
    if (success_percent > 70 && success_percent <= 90)
        printf("\e[33mSuccess rate of \e[1;34m%.2f%%\e[0m !\n"
        "The tests were mostly good, but there are still some fixing to do !\e[0m\n", success_percent);
    if (success_percent > 90 && success_percent <= 100)
        printf("\e[32mSuccess rate of \e[1;34m%.2f%%\e[0m !\n"
        "Tests suite find only a small percent of probleme, it should be quick to fix !\e[0m\n", success_percent);
    float total_ms = 0;
    for (struct criterion_test_stats *ts = suite_stat->tests; ts; ts = ts->next)
        total_ms += ts->elapsed_time;
    printf("Suite completed in %.4f second\n", total_ms);
}
