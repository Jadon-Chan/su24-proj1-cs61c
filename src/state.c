#include "state.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

/* Helper function definitions */
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t *state, unsigned int snum);
static char next_square(game_state_t *state, unsigned int snum);
static void update_tail(game_state_t *state, unsigned int snum);
static void update_head(game_state_t *state, unsigned int snum);

/* Task 1 */
game_state_t *create_default_state() {
  snake_t *snake_p = malloc(sizeof(snake_t));
  snake_p->head_col = 4;
  snake_p->head_row = 2;
  snake_p->tail_col = 2;
  snake_p->tail_row = 2;
  snake_p->live = true;

  game_state_t *game_state_p = malloc(sizeof(game_state_t));
  game_state_p->num_snakes = 1;
  game_state_p->num_rows = 18;
  game_state_p->snakes = snake_p;
  game_state_p->board = malloc(18 * sizeof(char *));

  for (int i = 0; i < 18; i++) {
	  game_state_p->board[i] = malloc(21 * sizeof(char));
  }
  strcpy(game_state_p->board[0], "####################");
  strcpy(game_state_p->board[17], "####################");
  strcpy(game_state_p->board[2], "# d>D    *         #");
  for (int i = 1; i < 17; i++) {
	  if (i != 2) {
		  strcpy(game_state_p->board[i], "#                  #");
	  }
  }

  return game_state_p;
}

/* Task 2 */
void free_state(game_state_t *state) {
  for (int i = 0; i < state->num_rows; i++) {
	  free(state->board[i]);
  }
  free(state->board);
  free(state->snakes);
  free(state);
  return;
}

/* Task 3 */
void print_board(game_state_t *state, FILE *fp) {
  for (int i = 0; i < state->num_rows; i++) {
	  fprintf(fp, "%s\n", state->board[i]);
  }
  return;
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t *state, char *filename) {
  FILE *f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t *state, unsigned int row, unsigned int col) { return state->board[row][col]; }

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch) {
  state->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
  if (c == 'w' || c == 'a' || c == 's' || c == 'd')
	  return true;
  else
	  return false;
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
  if (c == 'W' || c == 'A' || c == 'S' || c == 'D' || c == 'x')
	  return true;
  else
	  return false;
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c) {
  if (is_head(c) || is_tail(c) || c == '^' || c == '<' || c == 'v' || c == '>')
	  return true;
  else
	  return false;
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
  char ret_val = c;
  if (c == '^')
	  ret_val = 'w';
  else if (c == '<')
	  ret_val = 'a';
  else if (c == 'v')
	  ret_val = 's';
  else if (c == '>')
	  ret_val = 'd';
  return ret_val;
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c) {
  char ret_val = c;
  if (c == 'W')
	  ret_val = '^';
  else if (c == 'A')
	  ret_val = '<';
  else if (c == 'S')
	  ret_val = 'v';
  else if (c == 'D')
	  ret_val = '>';
  return ret_val;
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c) {
  unsigned int ret_val = cur_row;
  if (c == 'v' || c == 's' || c == 'S')
	  ret_val++;
  else if (c == '^' || c == 'w' || c == 'W')
	  ret_val--;
  return ret_val;
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c) {
  unsigned int ret_val = cur_col;
  if (c == '>' || c == 'd' || c == 'D')
	  ret_val++;
  else if (c == '<' || c == 'a' || c == 'A')
	  ret_val--;
  return ret_val;
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t *state, unsigned int snum) {
  unsigned int curr_row = state->snakes[snum].head_row;
  unsigned int curr_col = state->snakes[snum].head_col;
  char head = get_board_at(state, curr_row, curr_col);

  unsigned int next_row = get_next_row(curr_row, head);
  unsigned int next_col = get_next_col(curr_col, head);
  char next_head = get_board_at(state, next_row, next_col);

  return next_head;
}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t *state, unsigned int snum) {
  unsigned int curr_row = state->snakes[snum].head_row;
  unsigned int curr_col = state->snakes[snum].head_col;
  char head = get_board_at(state, curr_row, curr_col);

  unsigned int next_row = get_next_row(curr_row, head);
  unsigned int next_col = get_next_col(curr_col, head);
  set_board_at(state, next_row, next_col, head);
  set_board_at(state, curr_row, curr_col, head_to_body(head));

  state->snakes[snum].head_col = next_col;
  state->snakes[snum].head_row = next_row;
  
  return;
}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t *state, unsigned int snum) {
  unsigned int curr_row = state->snakes[snum].tail_row;
  unsigned int curr_col = state->snakes[snum].tail_col;
  char tail = get_board_at(state, curr_row, curr_col);

  unsigned int next_row = get_next_row(curr_row, tail);
  unsigned int next_col = get_next_col(curr_col, tail);
  char next_to_tail = get_board_at(state, next_row, next_col);

  set_board_at(state, next_row, next_col, body_to_tail(next_to_tail));
  set_board_at(state, curr_row, curr_col, ' ');

  state->snakes[snum].tail_col = next_col;
  state->snakes[snum].tail_row = next_row;
  return;
}

/* Task 4.5 */
void update_state(game_state_t *state, int (*add_food)(game_state_t *state)) {
  // TODO: Implement this function.
  for (unsigned int snum = 0; snum < state->num_snakes; snum++) {
	  char dest = next_square(state, snum);
	  if (state->snakes[snum].live == false) {
		  continue;
	  }
	  if (dest == '#' || is_snake(dest)) {
		  unsigned int curr_row = state->snakes[snum].head_row;
		  unsigned int curr_col = state->snakes[snum].head_col;
		  set_board_at(state, curr_row, curr_col, 'x');
		  state->snakes[snum].live = false;
	  }
	  else if (dest == '*') {
		  update_head(state, snum);
		  add_food(state);
	  }
	  else {
		  update_head(state, snum);
		  update_tail(state, snum);
	  }
  } 
  return;
}

/* Task 5.1 */
char *read_line(FILE *fp) {
  char *tmp = malloc(200000 * sizeof(char));
  if (tmp && fgets(tmp, 200000, fp)) {
	  size_t length = strlen(tmp);
	  if ((tmp = realloc(tmp, (length+1) * sizeof(char)))) {
		  return tmp;
	  }
  }
  return NULL;
}

/* Task 5.2 */
game_state_t *load_board(FILE *fp) {
  unsigned int count = 0;
  char **tmp = malloc(200000 * sizeof(char *));
  char *line;
  while ((line = read_line(fp))) {
	  size_t length = strlen(line);
	  if (line[length-1] == '\n') {
		  line[length-1] = '\0';
	  }
	  tmp[count] = line;
	  count++;
  }
  game_state_t *state_p = malloc(sizeof(game_state_t));
  if ((tmp = realloc(tmp, count * sizeof(char *)))) {
	  state_p->num_rows = count;
	  state_p->num_snakes = 0;
	  state_p->snakes = NULL;
	  state_p->board = tmp;
	  return state_p;
  }
  return NULL;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_state_t *state, unsigned int snum) {
  unsigned int curr_row = state->snakes[snum].tail_row;
  unsigned int curr_col = state->snakes[snum].tail_col;
  char check = get_board_at(state, curr_row, curr_col);
  while (!is_head(check)) {
	  curr_row = get_next_row(curr_row, check);
	  curr_col = get_next_col(curr_col, check);
	  check = get_board_at(state, curr_row, curr_col);
  }
  state->snakes[snum].head_row = curr_row;
  state->snakes[snum].head_col = curr_col;
  return;
}

/* Task 6.2 */
game_state_t *initialize_snakes(game_state_t *state) {
  // TODO: Implement this function.
  unsigned int count = 0;
  unsigned int rows[200000];
  unsigned int cols[200000];
  for (unsigned int row = 0; row < state->num_rows; row++) {
	  for (unsigned int col = 0; col < strlen(state->board[row]); col++) {
		  char check = get_board_at(state, row, col);
		  if (is_tail(check)) {
			  rows[count] = row;
			  cols[count] = col;
			  count++;
		  }
	  }
  }
  state->snakes = malloc(count * sizeof(snake_t));
  for (unsigned int i = 0; i < count; i++) {
	  state->snakes[i].tail_row = rows[i];
	  state->snakes[i].tail_col = cols[i];
	  state->snakes[i].live = true;
	  find_head(state, i);
  }
  state->num_snakes = count;
  return state;
}
