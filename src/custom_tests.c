#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asserts.h"
// Necessary due to static functions in state.c
#include "state.c"

/* Look at asserts.c for some helpful assert functions */

int greater_than_forty_two(int x) { return x > 42; }

bool is_vowel(char c) {
  char *vowels = "aeiouAEIOU";
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
  for (char testcase = '\0'; testcase >= 0; testcase++) {
	  if (testcase == 'w' || testcase == 'a' || testcase == 's' || testcase == 'd') {
		  if (!assert_true("Failed on wasd test", is_tail(testcase))) {
		  	  return false;
		  }
	  }
	  else {
		  if (!assert_false("Failed on non-wasd test", is_tail(testcase))) {
			  return false;
		  }
	  }
  }
  return true;
}

bool test_is_head() {
  for (char testcase = '\0'; testcase >= 0; testcase++) {
	  if (testcase == 'W' || testcase == 'A' || testcase == 'S' || testcase == 'D' || testcase == 'x') {
		  if (!assert_true("Failed on WASDx test", is_head(testcase))) {
		  	  return false;
		  }
	  }
	  else {
		  if (!assert_false("Failed on non-WASDx test", is_head(testcase))) {
			  return false;
		  }
	  }
  }
  return true;
}

bool test_is_snake() {
  for (char testcase = '\0'; testcase >= 0; testcase++) {
	  if (testcase == 'W' || testcase == 'A' || testcase == 'S' || testcase == 'D' || testcase == 'x' || testcase == 'w' || testcase == 'a' || testcase == 's' || testcase == 'd' || testcase == '^' || testcase == '<' || testcase == 'v' || testcase == '>') {
		  if (!assert_true("Failed on WASDxwasd^<v> test", is_snake(testcase))) {
		  	  return false;
		  }
	  }
	  else {
		  if (!assert_false("Failed on non-WASDxwasd^<v> test", is_snake(testcase))) {
			  return false;
		  }
	  }
  }
  return true;
}

bool test_body_to_tail() {
  char body[5] = "^<v>";
  char tail[5] = "wasd";
  for (int i = 0; i < 4; i++) {
	  if (!assert_equals_char("Failed on body_to_tail", tail[i], body_to_tail(body[i]))) {
		  return false;
	  }
  }
  return true;
}

bool test_head_to_body() {
  char body[5] = "^<v>";
  char head[5] = "WASD";
  for (int i = 0; i < 4; i++) {
	  if (!assert_equals_char("Failed on head_to_body", body[i], head_to_body(head[i]))) {
		  return false;
	  }
  }
  return true;
}

bool test_get_next_row() {
  char up[4] = "^wW";
  char down[4] = "vsS";
  unsigned int random_row = 1 + rand();
  for (int i = 0; i < 3; i++) {
	  if (!assert_equals_unsigned_int("Failed on up", random_row-1, get_next_row(random_row, up[i]))) {
		  return false;
	  }
	  if (!assert_equals_unsigned_int("Failed on down", random_row+1, get_next_row(random_row, down[i]))) {
		  return false;
	  }
  }
  return true;
}

bool test_get_next_col() {
  char left[4] = "<aA";
  char right[4] = ">dD";
  unsigned int random_col = 1 + rand();
  for (int i = 0; i < 3; i++) {
	  if (!assert_equals_unsigned_int("Failed on left", random_col-1, get_next_col(random_col, left[i]))) {
		  return false;
	  }
	  if (!assert_equals_unsigned_int("Failed on right", random_col+1, get_next_col(random_col, right[i]))) {
		  return false;
	  }
  }
  return true;
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
  if (!test_get_next_row()) {
    printf("%s\n", "test_get_next_row failed");
    return false;
  }
  if (!test_get_next_col()) {
    printf("%s\n", "test_get_next_col failed");
    return false;
  }
  return true;
}

int main(int argc, char *argv[]) {
  init_colors();

  if (!test_and_print("custom", test_customs)) {
    return 0;
  }

  return 0;
}
