#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <signal.h>

#include <sys/types.h>

#include <sys/wait.h>

#include <pthread.h>

#include <stdbool.h>

#include <time.h>

#include <semaphore.h>

#define NUM_PLAYERS 4
#define BOARD_SIZE 52
#define PIECES_PER_PLAYER 4
#define HOME_STRETCH 6
#define YARD_SIZE 4
#define SAFE_SQUARES 8

// Function prototypes
void initialize_game();
void * player_turn(void * arg);
int roll_dice();
void move_piece(int player, int piece, int steps);
void handle_signal(int sig);
void print_board();
bool check_win(int player);
void update_grid();
void display_winner();

// Global variables
int board[BOARD_SIZE];
int player_positions[NUM_PLAYERS][PIECES_PER_PLAYER];
int yards[NUM_PLAYERS][YARD_SIZE];
int current_player = 0;
pthread_mutex_t game_mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t dice_semaphore;
sem_t grid_semaphore;
bool game_over = false;
int winner = -1;
int hit_count[NUM_PLAYERS] = {
  0
};
int cancelled_threads = 0;

// Safe square positions (including starting positions)
const int safe_squares[SAFE_SQUARES] = {
  0,
  13,
  26,
  39,
  8,
  21,
  34,
  47
};

int main() {
  srand(time(NULL));
  initialize_game();

  // Set up signal handler
  signal(SIGINT, handle_signal);

  // Initialize semaphores
  sem_init( & dice_semaphore, 0, 1);
  sem_init( & grid_semaphore, 0, 1);

  pthread_t player_threads[NUM_PLAYERS];

  // Create threads for each player
  for (int i = 0; i < NUM_PLAYERS; i++) {
    int * player_id = (int *)malloc(sizeof(int));
    * player_id = i;
    if (pthread_create( & player_threads[i], NULL, player_turn, player_id) != 0) {
      perror("Failed to create thread");
      exit(1);
    }
  }

  // Main thread displays the grid and token placement
  while (!game_over) {
    sem_wait( & grid_semaphore);
    print_board();
    sem_post( & grid_semaphore);
    sleep(1);
  }

  // Wait for all threads to finish
  for (int i = 0; i < NUM_PLAYERS; i++) {
    pthread_join(player_threads[i], NULL);
  }

  display_winner();

  // Clean up semaphores
  sem_destroy( & dice_semaphore);
  sem_destroy( & grid_semaphore);

  return 0;
}

void initialize_game() {
  // Initialize the board, player positions, and yards
  for (int i = 0; i < BOARD_SIZE; i++) {
    board[i] = -1;
  }

  for (int i = 0; i < NUM_PLAYERS; i++) {
    for (int j = 0; j < PIECES_PER_PLAYER; j++) {
      player_positions[i][j] = -1; // -1 indicates piece is not on the board
      yards[i][j] = j; // Initialize all pieces in the yard
    }
  }
}

void * player_turn(void * arg) {
  int player_id = * (int * ) arg;
  free(arg);

  while (!game_over) {
    pthread_mutex_lock( & game_mutex);

    if (current_player == player_id) {
      printf("\nPlayer %d's turn\n", player_id + 1);

      // Roll dice (with semaphore protection)
      sem_wait( & dice_semaphore);
      int dice_roll = roll_dice();
      sem_post( & dice_semaphore);

      printf("Player %d rolled a %d\n", player_id + 1, dice_roll);

      // Check if player can move any piece
      bool can_move = false;
      for (int i = 0; i < PIECES_PER_PLAYER; i++) {
        if ((player_positions[player_id][i] == -1 && dice_roll == 6) ||
          (player_positions[player_id][i] >= 0 &&
            player_positions[player_id][i] + dice_roll < BOARD_SIZE + HOME_STRETCH)) {
          can_move = true;
          break;
        }
      }

      if (can_move) {
        int piece_to_move = -1;
        // Display all movable pieces
        printf("Player %d, here are your pieces:\n", player_id + 1);
        for (int i = 0; i < PIECES_PER_PLAYER; i++) {
          if ((player_positions[player_id][i] == -1 && dice_roll == 6) ||
            (player_positions[player_id][i] >= 0 &&
              player_positions[player_id][i] + dice_roll < BOARD_SIZE + HOME_STRETCH)) {
            printf("Piece %d: Position %d (Movable)\n", i, player_positions[player_id][i]);
          } else {
            printf("Piece %d: Position %d (Not Movable)\n", i, player_positions[player_id][i]);
          }
        }

        for (int i = 0; i < PIECES_PER_PLAYER; i++) {
          if ((player_positions[player_id][i] == -1 && dice_roll == 6) ||
            (player_positions[player_id][i] >= 0 &&
              player_positions[player_id][i] + dice_roll < BOARD_SIZE + HOME_STRETCH)) {
            piece_to_move = i;
            break;
          }
        }

        if (piece_to_move == -1) {
          printf("No valid moves for Player %d. Skipping turn.\n", player_id + 1);
          current_player = (current_player + 1) % NUM_PLAYERS;
          pthread_mutex_unlock( & game_mutex);
          continue;
        }

        printf("Player %d moves piece %d\n", player_id + 1, piece_to_move);

        move_piece(player_id, piece_to_move, dice_roll);

        if (check_win(player_id)) {
          winner = player_id;
          game_over = true;
        }
      } else {
        printf("No valid moves. Skipping turn.\n");
        current_player = (current_player + 1) % NUM_PLAYERS;
      }

      // Bonus roll for rolling a 6
      if (dice_roll == 6 && !game_over) {
        printf("Bonus roll for Player %d!\n", player_id + 1);
      } else {
        current_player = (current_player + 1) % NUM_PLAYERS;
      }

      // Update grid after each move
      update_grid();
    }

    pthread_mutex_unlock( & game_mutex);
    sleep(1); // Prevent busy waiting
  }

  cancelled_threads++;
  return NULL;
}

int roll_dice() {
  return (rand() % 6) + 1;
}

void move_piece(int player, int piece, int steps) {
  int current_position = player_positions[player][piece];

  if (current_position == -1) {
    if (steps == 6) {
      // Move piece from yard to starting position
      player_positions[player][piece] = player * (BOARD_SIZE / NUM_PLAYERS);
      yards[player][piece] = -1; // Remove from yard
      printf("Player %d's piece %d is now on the board\n", player + 1, piece);
    } else {
      printf("Cannot move piece onto the board without rolling a 6\n");
      return;
    }
  } else {
    int new_position = (current_position + steps) % BOARD_SIZE;

    // Check if the piece is entering its home stretch
    if (new_position >= player * (BOARD_SIZE / NUM_PLAYERS) + BOARD_SIZE / NUM_PLAYERS) {
      int home_position = new_position - (player * (BOARD_SIZE / NUM_PLAYERS) + BOARD_SIZE / NUM_PLAYERS);
      if (home_position < HOME_STRETCH) {
        player_positions[player][piece] = BOARD_SIZE + home_position;
        printf("Player %d's piece %d is in the home stretch at position %d\n", player + 1, piece, home_position);
      } else {
        printf("Cannot move beyond home. Piece stays in place.\n");
        return;
      }
    } else {
      // Check if the new position is occupied by another player's piece
      for (int p = 0; p < NUM_PLAYERS; p++) {
        if (p != player) {
          for (int pc = 0; pc < PIECES_PER_PLAYER; pc++) {
            if (player_positions[p][pc] == new_position) {
              // Check if the square is safe
              bool is_safe = false;
              for (int i = 0; i < SAFE_SQUARES; i++) {
                if (new_position == safe_squares[i]) {
                  is_safe = true;
                  break;
                }
              }

              if (!is_safe) {
                player_positions[p][pc] = -1;
                yards[p][pc] = pc; // Return to yard
                hit_count[player]++;
                printf("Player %d's piece was sent back to yard!\n", p + 1);
              } else {
                printf("Cannot move to occupied safe square. Piece stays in place.\n");
                return;
              }
            }
          }
        }
      }

      player_positions[player][piece] = new_position;
    }
  }

  printf("Player %d's piece %d moved to position %d\n", player + 1, piece, player_positions[player][piece]);
}

void handle_signal(int sig) {
  if (sig == SIGINT) {
    printf("\nGame interrupted. Cleaning up...\n");
    game_over = true;
    exit(0);
  }
}

void print_board() {
  printf("\nCurrent board state:\n");
  for (int i = 0; i < BOARD_SIZE; i++) {
    int occupied = -1;
    for (int p = 0; p < NUM_PLAYERS; p++) {
      for (int pc = 0; pc < PIECES_PER_PLAYER; pc++) {
        if (player_positions[p][pc] == i) {
          occupied = p;
          break;
        }
      }
      if (occupied != -1) break;
    }

    if (occupied != -1) {
      printf("[P%d]", occupied + 1);
    } else {
      bool is_safe = false;
      for (int s = 0; s < SAFE_SQUARES; s++) {
        if (i == safe_squares[s]) {
          is_safe = true;
          break;
        }
      }
      printf(is_safe ? "[S]" : "[ ]");
    }

    if ((i + 1) % (BOARD_SIZE / NUM_PLAYERS) == 0) {
      printf("\n");
    }
  }

  printf("\nHome stretches:\n");
  for (int p = 0; p < NUM_PLAYERS; p++) {
    printf("Player %d: ", p + 1);
    for (int h = 0; h < HOME_STRETCH; h++) {
      bool occupied = false;
      for (int pc = 0; pc < PIECES_PER_PLAYER; pc++) {
        if (player_positions[p][pc] == BOARD_SIZE + h) {
          printf("[X]");
          occupied = true;
          break;
        }
      }
      if (!occupied) {
        printf("[ ]");
      }
    }
    printf("\n");
  }

  printf("\nYards:\n");
  for (int p = 0; p < NUM_PLAYERS; p++) {
    printf("Player %d: ", p + 1);
    for (int y = 0; y < YARD_SIZE; y++) {
      if (yards[p][y] != -1) {
        printf("[O]");
      } else {
        printf("[ ]");
      }
    }
    printf("\n");
  }
}

bool check_win(int player) {
  for (int i = 0; i < PIECES_PER_PLAYER; i++) {
    if (player_positions[player][i] < BOARD_SIZE + HOME_STRETCH - 1) {
      return false;
    }
  }
  return true;
}

void update_grid() {
  sem_wait( & grid_semaphore);
  // The grid is already updated in the global variables
  // We just need to trigger a redraw
  print_board();
  sem_post( & grid_semaphore);
}

void display_winner() {
  printf("\nGame Over!\n");
  printf("Winner: Player %d\n", winner + 1);
  printf("Hit rates:\n");
  for (int i = 0; i < NUM_PLAYERS; i++) {
    printf("Player %d: %d hits\n", i + 1, hit_count[i]);
  }
  printf("Cancelled threads: %d\n", cancelled_threads);
}