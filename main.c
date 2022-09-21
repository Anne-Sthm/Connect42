#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define ROW_NUM 6
#define COL_NUM 7
#define MAX_NAME_LENGTH 50

typedef enum Token { EMPTY = 0, RED = 1, YELLOW = 2 } Token;

typedef enum GameState {
  ONGOING = 0,
  RED_WINS = 1,
  YELLOW_WINS = 2,
  TIE = 3
} GameState;

typedef struct Game {
  Token grid[ROW_NUM][COL_NUM];
  int current_player;
  char player1[MAX_NAME_LENGTH];
  char player2[MAX_NAME_LENGTH];
  GameState game_state;
} Game;

// Takes as an input the coordinates of the last dropped token
// Assumes that the game was ongoing before the last drop of a token
GameState game_check_state(Game *game, int x) {
  int y = 0;
  while (y < ROW_NUM - 1 && game->grid[y][x] != EMPTY) {
    y++;
  }
  y--;

  // Check vertical
  if (y >= 3 && game->grid[y][x] == game->grid[y - 1][x] &&
      game->grid[y - 1][x] == game->grid[y - 2][x] &&
      game->grid[y - 2][x] == game->grid[y - 3][x]) {
    return (game->grid[y][x] == RED) ? RED_WINS : YELLOW_WINS;
  }
  // Check horizontal
  int counter = 0;
  for (int temp_x = 0; temp_x < COL_NUM; temp_x++) {
    if (game->grid[y][temp_x] == game->grid[y][x]) {
      counter++;
    } else {
      counter = 0;
    }
    if (counter == 4) {
      return (game->grid[y][x] == RED) ? RED_WINS : YELLOW_WINS;
    }
  }

  // Check diagonal
  int counter_asc = 0;
  int counter_des = 0;
  for (int temp = -3; temp <= 3; temp++) {
    if (game->grid[y + temp][x + temp] == game->grid[y][x]) {
      counter_asc++;
    } else {
      counter_asc = 0;
    }
    if (game->grid[y + temp][x - temp] == game->grid[y][x]) {
      counter_des++;
    } else {
      counter_des = 0;
    }
    if (counter_asc == 4 || counter_des == 4) {
      return (game->grid[y][x] == RED) ? RED_WINS : YELLOW_WINS;
    }
  }

  // Check tie
  for (int temp_x; temp_x < COL_NUM; temp_x++) {
    if (game->grid[ROW_NUM - 1][temp_x] == EMPTY) {
      return ONGOING;
    }
  }
  return TIE;
}

// will add the token, returns true if it works
bool game_put_token(Game *game, int x, int current_player) {
  for (int y = 0; y < ROW_NUM; y++) {
    if (game->grid[y][x] == EMPTY) {
      game->grid[y][x] = (current_player == 1) ? RED : YELLOW;
      return true;
    }
  }
  return false;
}

void game_show(Game *game) {
  printf("| 1 | 2 | 3 | 4 | 5 | 6 | 7 |\n");
  printf("| - + - + - + - + - + - + - |\n");
  for (int y = ROW_NUM - 1; y >= 0; y--) {
    printf("|");
    for (int x = 0; x < COL_NUM; x++) {
      printf(" %d |", game->grid[y][x]);
    }
    printf("\n");
    printf("| - + - + - + - + - + - + - |\n");
  }
}

void game_init(Game *game) {
  // Initializing the grid
  for (int i = 0; i < ROW_NUM; i++) {
    for (int j = 0; j < COL_NUM; j++) {
      game->grid[i][j] = EMPTY;
    }
  }
  // Initializing the players
  printf("--------/Welcome to Connect 4/--------\n\n");
  printf("Player 1, please enter your name: ");
  scanf("%s", game->player1);
  printf("Player 2, please enter your name: ");
  scanf("%s", game->player2);
  printf("\n");

  game->game_state = ONGOING;
  game->current_player = 1;
}

void game_run_turn(Game *game) {
  int chosen_col = 0;
  game_show(game);
  // Asks for input and drops the token
  printf("%s, choose a column to drop your token in: ",
         (game->current_player == 1) ? game->player1 : game->player2);
  scanf("%d", &chosen_col);
  chosen_col--;
  while (!game_put_token(game, chosen_col, game->current_player)) {
    printf("Column %d is full, please enter another column: ", chosen_col + 1);
    scanf("%d", &chosen_col);
    chosen_col--;
  };
  game->current_player = (game->current_player == 1) ? 2 : 1;
  game->game_state = game_check_state(game, chosen_col);
}

void game_run(Game *game) {
  while (game->game_state == ONGOING) {
    game_run_turn(game);
  }

  game_show(game);
  switch (game->game_state) {
  case ONGOING:
    printf("ERROR: the game is not over yet\n");
    break;
  case RED_WINS:
    printf("RED WON!\n");
    break;
  case YELLOW_WINS:
    printf("YELLOW WON!\n");
    break;
  case TIE:
    printf("TIE!\n");
    break;
  }
}

int main() {
  Game *game = calloc(1, sizeof(Game));
  game_init(game);
  game_run(game);
  free(game);
  return 0;
}