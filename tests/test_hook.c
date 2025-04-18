/*
** EPITECH PROJECT, 2025
** test_hook.c
** File description:
** hook for the test
*/

#include <criterion/hooks.h>
#include <criterion/logging.h>
#include <criterion/stats.h>
#include <criterion/types.h>
#include <stddef.h>
#include <stdio.h>

ReportHook(POST_ALL)(struct criterion_global_stats *stat)
{
    size_t i = 0;
    double success_percent;

    printf("\n");
    for (struct criterion_suite_stats *suite_stat = stat->suites; i < stat->nb_suites; suite_stat = suite_stat->next) {

        success_percent = -1;

        printf("Actual test suite is \e[1;34m%s\e[0m\n"
            "Description of this suite :\n%s",
            suite_stat->suite->name, suite_stat->suite->data->description);
            printf("\nResult of the test suite \e[1;34m%s\e[0m\n"
                "\tNumber of tests : \e[1;34m%li\e[0m\n"
                "\t\e[36mTests that passed :\e[0m \e[1;34m%li\e[0m\n"
                "\t\e[31mTests that failed :\e[0m \e[1;34m%li\e[0m\n"
                "\t\tOf these, tests that \e[31mfailed normaly\e[0m : \e[1;34m%li\e[0m\n"
                "\t\tOf these, tests that \e[31mfailed with a crash\e[0m : \e[1;34m%li\e[0m\n"
                "\t\tOf these, tests that \e[33mfailed with a skip\e[0m : \e[1;34m%li\e[0m\n"
                "\tNumber of assert : \e[1;34m%li\e[0m\n"
                "\t\t\e[36mOf these, assert that passed :\e[0m  \e[1;34m%li\e[0m\n"
                "\t\t\e[31mOf these, assert that failed :\e[0m  \e[1;34m%li\e[0m\n"
                , suite_stat->suite->name, suite_stat->nb_tests,
                suite_stat->tests_passed,
                suite_stat->tests_skipped + suite_stat->tests_failed,
                suite_stat->tests_failed - (suite_stat->tests_crashed + suite_stat->tests_skipped),
                suite_stat->tests_crashed, suite_stat->tests_skipped,
                suite_stat->asserts_passed + suite_stat->asserts_failed, suite_stat->asserts_passed, suite_stat->asserts_failed);


            for (struct criterion_test_stats *stat_test = suite_stat->tests; stat_test; stat_test = stat_test->next) {
                if (stat_test->test_status != CR_STATUS_PASSED)
                    printf("Test \e[1;35m%s\e[0m of this suite failed...\n", stat_test->test->name);
            }

            if (suite_stat->nb_tests != 0)
                success_percent = ((double)(suite_stat->tests_passed) / (suite_stat->nb_tests - suite_stat->tests_skipped)) * 100;
            if (success_percent < 0 || success_percent > 100)
                printf("Unable to compute an percentage of success\n");
            if (success_percent >= 0 && success_percent <= 70)
                printf("\e[31mSuccess rate of \e[1;34m%.2f%%\e[0m !\n"
                "You might want to check back your code and fix these issue !\e[0m\n", success_percent);
            if (success_percent > 70 && success_percent <= 90)
                printf("\e[33mSuccess rate of \e[1;34m%.2f%%\e[0m !\n"
                "The tests were mostly good, but there are still some fixing to do !\e[0m\n", success_percent);
            if (success_percent > 90 && success_percent < 100)
                printf("\e[32mSuccess rate of \e[1;34m%.2f%%\e[0m !\n"
                "Tests suite find only a small percent of probleme, it should be quick to fix !\e[0m\n", success_percent);
            if (success_percent == 100)
                printf("\e[32mSuccess rate of \e[1;34m%.2f%%\e[0m !\n"
                "Tests suite find no probleme at all, well done !\e[0m\n", success_percent);

            float max_second = 0;

            for (struct criterion_test_stats *stat_test = suite_stat->tests; stat_test; stat_test = stat_test->next)
                if (max_second < stat_test->elapsed_time)
                    max_second = stat_test->elapsed_time;

            printf("Suite completed in %.4f second\n\n", max_second);
        i++;
    }
}
