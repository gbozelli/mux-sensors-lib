int test_1(){
  // ...
}

int main(){
  printf("Starting test...")
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