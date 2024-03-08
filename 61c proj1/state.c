#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snake_utils.h"
#include "state.h"

/* Helper function definitions */
static void set_board_at(game_state_t* state, unsigned int x, unsigned int y, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_x(unsigned int cur_x, char c);
static unsigned int get_next_y(unsigned int cur_y, char c);
static void find_head(game_state_t* state, unsigned int snum);
static char next_square(game_state_t* state, unsigned int snum);
static void update_tail(game_state_t* state, unsigned int snum);
static void update_head(game_state_t* state, unsigned int snum);


/* Task 1 */
game_state_t* create_default_state() {
  game_state_t* default_game = (game_state_t*) malloc(sizeof(game_state_t));
  default_game->num_rows = 18;
  default_game->num_snakes = 1;
  default_game->board = (char**) malloc((default_game->num_rows) * sizeof(char*));
  for (int i = 0; i < default_game->num_rows; i++) {
	 default_game->board[i] = (char*) malloc(21 * sizeof(char));
  }
  strcpy(default_game->board[0], "####################");
  strcpy(default_game->board[1], "#                  #");
  strcpy(default_game->board[2], "# d>D    *         #");
  strcpy(default_game->board[3], "#                  #");
  strcpy(default_game->board[4], "#                  #");
  strcpy(default_game->board[5], "#                  #");
  strcpy(default_game->board[6], "#                  #");
  strcpy(default_game->board[7], "#                  #");
  strcpy(default_game->board[8], "#                  #");
  strcpy(default_game->board[9], "#                  #");
  strcpy(default_game->board[10], "#                  #");
  strcpy(default_game->board[11], "#                  #");
  strcpy(default_game->board[12], "#                  #");
  strcpy(default_game->board[13], "#                  #");
  strcpy(default_game->board[14], "#                  #");
  strcpy(default_game->board[15], "#                  #");
  strcpy(default_game->board[16], "#                  #");
  strcpy(default_game->board[17], "####################");
  default_game->snakes = (snake_t*) malloc(sizeof(snake_t));
  default_game->num_snakes = 1;
  default_game->snakes->live = true;
  default_game->snakes->head_x = 4;
  default_game->snakes->head_y = 2;
  default_game->snakes->tail_x = 2;
  default_game->snakes->tail_y = 2;
  return default_game;
}


/* Task 2 */
void free_state(game_state_t* state) {
	for (int i = 0; i < state->num_rows; i++) {
		free(state->board[i]);
 		state->board[i] = NULL;
	}
	free(state->board);
	state->board = NULL;
	free(state->snakes);
	state->snakes = NULL;
	free(state);
	state = NULL;
	return;
}

/* Task 3 */
void print_board(game_state_t* state, FILE* fp) {
       for (int i = 0; i < state->num_rows; i++) {
       	fprintf(fp, "%s%c", state->board[i], '\n');
       }		
       return;
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t* state, char* filename) {
  FILE* f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}


/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t* state, unsigned int x, unsigned int y) {
  return state->board[y][x];
}

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t* state, unsigned int x, unsigned int y, char ch) {
  state->board[y][x] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
  if (c == 'w' || c == 'a' || c == 's' || c == 'd') {
	  return true;
  }
  return false;
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
  if (c == 'W' || c == 'A' || c == 'S' || c == 'D' || c == 'x') {
  	return true;
  }
  return false;
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<>vWASDx"
*/
static bool is_snake(char c) {
  if (is_head(c) || is_tail(c) || c == '^' || c == 'v') {
	  return true;
  }
  if (c == '<' || c =='>') {
	  return true;
  }
  return false;
}

/*
  Converts a character in the snake's body ("^<>v")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
  if (c == '^') {
	  return 'w';
  }
  if (c == '<') {
	  return 'a';
  }
  if (c == 'v') {
	  return 's';
  }
  if (c == '>') {
	  return 'd';
  }
  return '?';
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<>v").
*/
static char head_to_body(char c) {
  if (c == 'W') {
	  return '^';
  }
  if (c == 'A') {
	  return '<';
  }
  if (c == 'S') {
	  return 'v';
  }
  if (c == 'D') {
	  return '>';
  }
  return '?';
}

/*
  Returns cur_x + 1 if c is '>' or 'd' or 'D'.
  Returns cur_x - 1 if c is '<' or 'a' or 'A'.
  Returns cur_x otherwise.
*/
static unsigned int get_next_x(unsigned int cur_x, char c) {
  if (c == '>' || c == 'd' || c == 'D') {
	  return cur_x + 1;
  }
  if (c == '<' || c == 'a' || c == 'A') {
	  return cur_x - 1;
  }
  return cur_x;
}

/*
  Returns cur_y + 1 if c is '^' or 'w' or 'W'.
  Returns cur_y - 1 if c is 'v' or 's' or 'S'.
  Returns cur_y otherwise.
*/
static unsigned int get_next_y(unsigned int cur_y, char c) {
  if (c == '^' || c == 'w' || c == 'W') {
	  return cur_y - 1;
  }
  if (c == 'v' || c == 's' || c == 'S') {
	  return cur_y + 1;
  }
  return cur_y;
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t* state, unsigned int snum) {
  unsigned int x = state->snakes[snum].head_x;
  unsigned int y = state->snakes[snum].head_y;
  char direction = get_board_at(state, x, y);
  return get_board_at(state, get_next_x(x, direction), get_next_y(y, direction));
}


/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the x and y coordinates of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t* state, unsigned int snum) {
  unsigned int x = state->snakes[snum].head_x;
  unsigned int y = state->snakes[snum].head_y;
  char direction = get_board_at(state, x, y);
  state->snakes[snum].head_x = get_next_x(x, direction);
  state->snakes[snum].head_y = get_next_y(y, direction);
  set_board_at(state, state->snakes[snum].head_x, state->snakes[snum].head_y, direction);
  set_board_at(state, x, y, head_to_body(direction));
  return;
}


/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^v<>) into a tail character (wasd)

  ...in the snake struct: update the x and y coordinates of the tail
*/
static void update_tail(game_state_t* state, unsigned int snum) {
  unsigned int x = state->snakes[snum].tail_x;
  unsigned int y = state->snakes[snum].tail_y;
  char direction = get_board_at(state, x, y);
  unsigned int new_x = get_next_x(x, direction);
  unsigned int new_y = get_next_y(y, direction);
  char new_direction = body_to_tail(get_board_at(state, new_x, new_y));
  set_board_at(state, x, y, ' ');
  set_board_at(state, new_x, new_y, new_direction);
  state->snakes[snum].tail_x = new_x;
  state->snakes[snum].tail_y = new_y; 
  return;
}


/* Task 4.5 */
void update_state(game_state_t* state, int (*add_food)(game_state_t* state)) {
  for (unsigned int i = 0; i < state->num_snakes; i++) {
	  snake_t* curr_snake = state->snakes + i;
	  if (curr_snake->live == true) {
		  char next = next_square(state, i);
		  if (next == '#' || is_snake(next)) {
			  set_board_at(state, curr_snake->head_x, curr_snake->head_y, 'x');
			  curr_snake->live = false;
		  }
		  else if (next == ' ') {
			  update_head(state, i);
			  update_tail(state, i);
		  }
		  else if (next == '*') {
			  update_head(state, i);
			  add_food(state);
		  }
	  }
  }
  return;
}


/* Task 5 */
game_state_t* load_board(char* filename) {
  game_state_t *loaded_game = (game_state_t*) malloc(sizeof(game_state_t));
  unsigned int rows = 0;
  char temp[1000000];
  FILE* file;
  file = fopen(filename, "r");
  if (file == NULL) {
	  return NULL;
  }
  while (!feof(file)) {
	  fgets(temp, 1000000, file);
	  rows += 1;
  }	  
  loaded_game->board = (char**) malloc(rows * sizeof(char*));
  loaded_game->num_rows = rows - 1;
  char row[1000000];
  file = fopen(filename, "r");
  unsigned int y = 0;
  long unsigned int row_len = 0;
  while (!feof(file)) {
    fgets(row, 1000000, file);
    row_len = strlen(row);
    loaded_game->board[y] = (char*) malloc(row_len * sizeof(char));
    for(int x = 0; x < row_len; x++) {
      loaded_game->board[y][x] = row[x];
    }
    loaded_game->board[y][row_len - 1] = '\0';
    y++;
  }

  fclose(file);
  return loaded_game;
}


/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail coordinates filled in,
  trace through the board to find the head coordinates, and
  fill in the head coordinates in the struct.
*/
static void find_head(game_state_t* state, unsigned int snum) {
  snake_t* curr_snake = state->snakes + snum;
  unsigned int curr_x = curr_snake->tail_x;
  unsigned int curr_y = curr_snake->tail_y;
  char curr_square = get_board_at(state, curr_x, curr_y);
  while(is_snake(curr_square)) {
	  if (is_head(curr_square)) {
	    curr_snake->head_x = curr_x;
	    curr_snake->head_y = curr_y;
	    break;
	  }
	  curr_x = get_next_x(curr_x, curr_square);
	  curr_y = get_next_y(curr_y, curr_square);
	  curr_square = get_board_at(state, curr_x, curr_y);
  }
  return;
}


/* Task 6.2 */
game_state_t* initialize_snakes(game_state_t* state) {
  unsigned int y_size = state->num_rows;
  unsigned int num_snake = 0;
  unsigned int x_pos = 0; 
  for (unsigned int y_pos = 0; y_pos < y_size; y_pos++) {
    x_pos = 0;
    char track = get_board_at(state, x_pos, y_pos);
    while (track != '\0') {
      if (is_tail(get_board_at(state, x_pos, y_pos))) {
	      num_snake += 1;
      }
      x_pos += 1;
      track = get_board_at(state, x_pos, y_pos);
    }

  }
  state->num_snakes = num_snake;
  state->snakes = (snake_t*) malloc(num_snake * sizeof(snake_t));
  unsigned int snum = 0;
  x_pos = 0;
  for (unsigned int y_pos = 0; y_pos < y_size; y_pos++) {
    x_pos = 0;
    char track = get_board_at(state, x_pos, y_pos);	  
    while (track != '\0') {
      if (is_tail(get_board_at(state, x_pos, y_pos))) {
        snake_t* curr = state->snakes + snum;
	curr->tail_x = x_pos;
	curr->tail_y = y_pos;
	find_head(state, snum);
        unsigned int xhead = curr->head_x;
	unsigned int yhead = curr->head_y;
	if (get_board_at(state, xhead, yhead) == 'x') {
		curr->live = false;
	}
	else {
		curr->live = true;
	}
	snum += 1;
      }
      x_pos += 1;
      track = get_board_at(state, x_pos, y_pos);
    }
  }
  return state;
}
