#include <stdio.h>

int run_test(int test_id);

int test_1() { return 1; }

int main() {
    printf("Starting test...\n");
    int number_tests = 3;
    int count_passed_tests = 0;
    for (int i = 0; i < number_tests; i++) {
        // Run test i
        if (run_test(i)) {
            count_passed_tests++;
        } else {
            printf("Test %d failed.\n", i);
        }
    }
    printf("Passed %d out of %d tests.\n", count_passed_tests, number_tests);

    return 0;
}

int run_test(int test_id) {
    return 1;  // Dummy implementation for now
}
