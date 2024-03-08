#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "asserts.h"
// Necessary due to static functions in state.c
#include "state.c"

char* COLOR_GREEN = "";
char* COLOR_RESET = "";

/* Look at asserts.c for some helpful assert functions */

int greater_than_forty_two(int x) {
  return x > 42;
}

bool is_vowel(char c) {
  char* vowels = "aeiouAEIOU";
  for (int i = 0; i < strlen(vowels); i++) {
    if (c == vowels[i]) {
      return true;
    }
  }
  return false;
}

/*
  Example 1: Returns true if all test cases pass. False otherwise.
    The function greater_than_forty_two(int x) will return true if x > 42. False otherwise.
    Note: This test is NOT comprehensive
*/
bool test_greater_than_forty_two() {
  int testcase_1 = 42;
  bool output_1 = greater_than_forty_two(testcase_1);
  if (!assert_false("output_1", output_1)) {
    return false;
  }

  int testcase_2 = -42;
  bool output_2 = greater_than_forty_two(testcase_2);
  if (!assert_false("output_2", output_2)) {
    return false;
  }

  int testcase_3 = 4242;
  bool output_3 = greater_than_forty_two(testcase_3);
  if (!assert_true("output_3", output_3)) {
    return false;
  }

  return true;
}

/*
  Example 2: Returns true if all test cases pass. False otherwise.
    The function is_vowel(char c) will return true if c is a vowel (i.e. c is a,e,i,o,u)
    and returns false otherwise
    Note: This test is NOT comprehensive
*/
bool test_is_vowel() {
  char testcase_1 = 'a';
  bool output_1 = is_vowel(testcase_1);
  if (!assert_true("output_1", output_1)) {
    return false;
  }

  char testcase_2 = 'e';
  bool output_2 = is_vowel(testcase_2);
  if (!assert_true("output_2", output_2)) {
    return false;
  }

  char testcase_3 = 'i';
  bool output_3 = is_vowel(testcase_3);
  if (!assert_true("output_3", output_3)) {
    return false;
  }

  char testcase_4 = 'o';
  bool output_4 = is_vowel(testcase_4);
  if (!assert_true("output_4", output_4)) {
    return false;
  }

  char testcase_5 = 'u';
  bool output_5 = is_vowel(testcase_5);
  if (!assert_true("output_5", output_5)) {
    return false;
  }

  char testcase_6 = 'k';
  bool output_6 = is_vowel(testcase_6);
  if (!assert_false("output_6", output_6)) {
    return false;
  }

  return true;
}

/* Task 4.1 */

bool test_is_tail() {
  bool test1 = is_tail('w');
  bool test2 = is_tail('e');
  bool test3 = is_tail('W');
  bool test4 = is_tail('x');
  bool test5 = is_tail(' ');
  return test1 && !test2 && !test3 && !test4 && !test5;
}

bool test_is_head() {
  bool test1 = is_head('w');
  bool test2 = is_head('e');
  bool test3 = is_head('W');
  bool test4 = is_head('x');
  bool test5 = is_head(' ');  
  return !test1 && !test2 && test3 && test4 && !test5;
}

bool test_is_snake() {
  bool test1 = is_snake('x');
  bool test2 = is_snake('<');
  bool test3 = is_snake('e');
  bool test4 = is_snake('W');
  bool test5 = is_snake(' ');
  return test1 && test2 && !test3 && test4 && !test5;
}

bool test_body_to_tail() {
  char test1 = body_to_tail('<');
  char test2 = body_to_tail('W');
  char test3 = body_to_tail('v');
  char test4 = body_to_tail('x');
  char test5 = body_to_tail(' ');
  if (test1 == 'a' && test2 == '?' && test3 == 's' && test4 == '?' && test5 == '?') {
	  return true;
  }
  return false;
}

bool test_head_to_body() {
  char test1 = head_to_body('W');
  char test2 = head_to_body('w');
  char test3 = head_to_body('S');
  char test4 = head_to_body('x');
  char test5 = head_to_body(' ');
  if (test1 == '^' && test2 == '?' && test3 == 'v' && test4 == '?' && test5 == '?') {
	  return true;
  }
  return false;
}

bool test_get_next_x() {
  unsigned int test1 = get_next_x(2, 'd');
  unsigned int test2 = get_next_x(2, 'A');
  unsigned int test3 = get_next_x(2, 'v');
  unsigned int test4 = get_next_x(2, ' ');
  unsigned int test5 = get_next_x(2, '>');
  if (test1 == 3 && test2 == 1 && test3 == 2 && test4 == 2 && test5 == 3) {
	  return true;
  }
  return false;
}

bool test_get_next_y() {
  unsigned int test1 = get_next_y(2, 'w');
  unsigned int test2 = get_next_y(2, 'S');
  unsigned int test3 = get_next_y(2, '<');
  unsigned int test4 = get_next_y(2, ' ');
  unsigned int test5 = get_next_y(2, 'v');
  if (test1 == 1 && test2 == 3 && test3  == 2 && test4 == 2 && test5 == 3) {
	  return true;
  }
  return false;
}

bool test_customs() {
  if (!test_greater_than_forty_two()) {
    printf("%s\n", "test_greater_than_forty_two failed.");
    return false;
  }
  if (!test_is_vowel()) {
    printf("%s\n", "test_is_vowel failed.");
    return false;
  }
  if (!test_is_tail()) {
    printf("%s\n", "test_is_tail failed");
    return false;
  }
  if (!test_is_head()) {
    printf("%s\n", "test_is_head failed");
    return false;
  }
  if (!test_is_snake()) {
    printf("%s\n", "test_is_snake failed");
    return false;
  }
  if (!test_body_to_tail()) {
    printf("%s\n", "test_body_to_tail failed");
    return false;
  }
  if (!test_head_to_body()) {
    printf("%s\n", "test_head_to_body failed");
    return false;
  }
  if (!test_get_next_x()) {
    printf("%s\n", "test_get_next_x failed");
    return false;
  }
  if (!test_get_next_y()) {
    printf("%s\n", "test_get_next_y failed");
    return false;
  }
  return true;
}

void init_colors() {
  if (!isatty(STDOUT_FILENO)) {
    return;
  }

  if (getenv("NO_COLOR") != NULL) {
    return;
  }

  char* term = getenv("TERM");
  if (term == NULL || strstr(term, "xterm") == NULL) {
    return;
  }

  COLOR_GREEN = "\033[0;32m";
  COLOR_RESET = "\033[0m";
}

bool test_and_print(char* label, bool (*run_test)()) {
  printf("\nTesting %s...\n", label);
  bool result = run_test();
  if (result) {
    printf("%sAll %s tests passed!%s\n", COLOR_GREEN, label, COLOR_RESET);
  } else {
    printf("Not all %s tests passed.\n", label);
  }
  return result;
}

int main(int argc, char* argv[]) {
  init_colors();

  if (!test_and_print("custom", test_customs)) {
    return 0;
  }

  return 0;
}
