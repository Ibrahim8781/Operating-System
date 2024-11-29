#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <random>
#include <map>
#include <chrono>
#include <algorithm>
#include <iomanip>

const int BOARD_SIZE = 52;
const int NUM_PLAYERS = 4;
const int MAX_TOKENS_PER_PLAYER = 4;
const int MAX_CONSECUTIVE_SIXES = 3;

// Enum to represent player colors
enum class PlayerColor {
    RED,
    GREEN,
    YELLOW,
    BLUE
};

// Structure to pass thread parameters
struct ThreadParams {
    int player_id;
    int num_tokens;
    std::vector<int>& tokens;
    std::mutex& game_mutex;
    std::condition_variable& turn_cv;
    bool* is_turn;
};

class LudoGame {
private:
    // Game board and resources
    std::vector<int> board;
    std::vector<std::vector<int>> player_tokens;
    std::vector<int> hit_records;
    
    // Synchronization primitives
    std::mutex board_mutex;
    std::mutex dice_mutex;
    std::condition_variable turn_cv;
    
    // Game state tracking
    int current_turn;
    std::vector<bool> player_completed;
    
    // Random number generation
    std::random_device rd;
    std::mt19937 gen;

public:
    LudoGame() : 
        board(BOARD_SIZE, -1),  // Initialize board with no tokens
        player_tokens(NUM_PLAYERS, std::vector<int>(MAX_TOKENS_PER_PLAYER, -1)),
        hit_records(NUM_PLAYERS, 0),
        current_turn(0),
        player_completed(NUM_PLAYERS, false),
        gen(rd()) 
    {}

    // Generate random dice roll (1-6)
    int rollDice() {
        std::uniform_int_distribution<> dis(1, 6);
        return dis(gen);
    }

    // Check if a token can be moved
    bool canMoveToken(int player, int token_index, int dice_roll) {
        int current_pos = player_tokens[player][token_index];
        
        // Token not in play yet
        if (current_pos == -1) {
            return dice_roll == 6;
        }
        
        // Calculate new position
        int new_pos = (current_pos + dice_roll) % BOARD_SIZE;
        
        // Check for blocking or safe squares
        return isValidMove(player, current_pos, new_pos);
    }

    bool isValidMove(int player, int current_pos, int new_pos) {
    // Safe square definitions for each player
    std::vector<int> safe_squares = {
        0, 8, 13, 21, 26, 34, 39, 47  // Example safe square positions
    };

    // Home column start positions for each player
    std::map<int, int> home_column_starts = {
        {0, 0},    // Red player's home column start
        {1, 13},   // Green player's home column start
        {2, 26},   // Yellow player's home column start
        {3, 39}    // Blue player's home column start
    };

    // Check if the move crosses any opponent's blocking
    for (int p = 0; p < NUM_PLAYERS; ++p) {
        if (p == player) continue;

        for (int t = 0; t < MAX_TOKENS_PER_PLAYER; ++t) {
            int opponent_pos = player_tokens[p][t];
            
            // Check for blocking condition
            if (opponent_pos != -1) {
                // If multiple tokens of same color are on consecutive squares
                int block_count = 0;
                for (int j = 0; j < MAX_TOKENS_PER_PLAYER; ++j) {
                    if (player_tokens[p][j] == opponent_pos) {
                        block_count++;
                    }
                }
                
                // If 2 tokens of same color are on same square, it's a block
                if (block_count >= 2 && current_pos < opponent_pos && new_pos > opponent_pos) {
                    return false;  // Cannot pass blocking tokens
                }
            }
        }
    }

    // Check for safe squares
    if (std::find(safe_squares.begin(), safe_squares.end(), new_pos) != safe_squares.end()) {
        return true;
    }

    // Check hit conditions
    for (int p = 0; p < NUM_PLAYERS; ++p) {
        if (p == player) continue;

        for (int t = 0; t < MAX_TOKENS_PER_PLAYER; ++t) {
            if (player_tokens[p][t] == new_pos) {
                // Cannot hit on safe squares
                if (std::find(safe_squares.begin(), safe_squares.end(), new_pos) != safe_squares.end()) {
                    return false;
                }
            }
        }
    }

    return true;
}


    void moveToken(int player, int token_index, int dice_roll) {
        std::lock_guard<std::mutex> lock(board_mutex);
        
        int& current_pos = player_tokens[player][token_index];
        
        // If token not in play, place it on starting square
        if (current_pos == -1 && dice_roll == 6) {
            current_pos = player * 13;  // Starting square for each player
            return;
        }
        
        // Move token
        int new_pos = (current_pos + dice_roll) % BOARD_SIZE;
        
        // Check for hitting opponent tokens
        for (int p = 0; p < NUM_PLAYERS; ++p) {
            if (p == player) continue;
            
            for (int t = 0; t < MAX_TOKENS_PER_PLAYER; ++t) {
                if (player_tokens[p][t] == new_pos) {
                    // Hit opponent token
                    player_tokens[p][t] = -1;
                    hit_records[p]++;
                }
            }
        }
        
        current_pos = new_pos;
    }

    void playerThread(ThreadParams& params) {
        while (true) {
            std::unique_lock<std::mutex> lock(params.game_mutex);
            
            // Wait for turn
            params.turn_cv.wait(lock, [&]{ return *params.is_turn; });
            
            // Roll dice
            int dice_roll = rollDice();
            
            // Implement token movement logic
            bool moved = false;
            for (int i = 0; i < params.num_tokens; ++i) {
                if (canMoveToken(params.player_id, i, dice_roll)) {
                    moveToken(params.player_id, i, dice_roll);
                    moved = true;
                    break;
                }
            }
            
            // Pass turn if no move possible
            if (!moved) {
                // Move to next player
            }
            
            // Check for game completion
            if (isPlayerCompleted(params.player_id)) {
                player_completed[params.player_id] = true;
                return;
            }
            
            *params.is_turn = false;
            params.turn_cv.notify_all();
        }
    }

    bool isPlayerCompleted(int player) {
    // Check if all tokens have reached home column
    int home_column_start = player * 13;
    int home_column_end = home_column_start + 5;  // Assuming 5 squares in home column

    int completed_tokens = 0;
    for (int i = 0; i < MAX_TOKENS_PER_PLAYER; ++i) {
        int token_pos = player_tokens[player][i];
        
        // Token is in home column and reached the end
        if (token_pos >= home_column_start && token_pos <= home_column_end) {
            completed_tokens++;
        }
    }

    // Player is completed if all tokens reached home
    return completed_tokens == MAX_TOKENS_PER_PLAYER;
}


void displayBoard() {
    // Create a 2D representation of the board
    std::vector<std::vector<char>> board_display(10, std::vector<char>(10, '.'));

    // Color representations
    std::map<int, char> player_colors = {
        {0, 'R'},  // Red
        {1, 'G'},  // Green
        {2, 'Y'},  // Yellow
        {3, 'B'}   // Blue
    };

    // Mark token positions
    for (int player = 0; player < NUM_PLAYERS; ++player) {
        for (int token = 0; token < MAX_TOKENS_PER_PLAYER; ++token) {
            int pos = player_tokens[player][token];
            
            if (pos != -1) {
                // Map board position to display grid
                int x = pos % 10;
                int y = pos / 10;
                board_display[y][x] = player_colors[player];
            }
        }
    }

    // Display board
    std::cout << "Ludo Board Current State:" << std::endl;
    for (const auto& row : board_display) {
        for (char cell : row) {
            std::cout << std::setw(2) << cell;
        }
        std::cout << std::endl;
    }

    // Display additional game statistics
    std::cout << "\nPlayer Statistics:" << std::endl;
    for (int player = 0; player < NUM_PLAYERS; ++player) {
        std::cout << "Player " << player + 1 
                  << " Hit Rate: " << hit_records[player] 
                  << " Completed: " << (player_completed[player] ? "Yes" : "No") 
                  << std::endl;
    }
}

    void startGame(int tokens_per_player) {
        std::vector<std::thread> player_threads;
        std::vector<char> player_turn(NUM_PLAYERS, false);
        std::mutex game_mutex;
        
        // Create thread parameters for each player
        std::vector<ThreadParams> thread_params;
        for (int i = 0; i < NUM_PLAYERS; ++i) {
            thread_params.push_back(ThreadParams{
                i, 
                tokens_per_player, 
                player_tokens[i],
                game_mutex,
                turn_cv,
                reinterpret_cast<bool*>(&player_turn[i])
            });
        }
        
        // Create player threads
        for (int i = 0; i < NUM_PLAYERS; ++i) {
            player_threads.emplace_back(&LudoGame::playerThread, this, std::ref(thread_params[i]));
        }
        
        // Game loop
        while (!isGameOver()) {
            // Randomly select current turn
            int current_player = selectNextPlayer();
            
            // Grant turn to selected player
            std::unique_lock<std::mutex> lock(game_mutex);
            player_turn[current_player] = true;
            turn_cv.notify_all();
            
            // Display board after each iteration
            displayBoard();
        }
        
        // Wait for all threads to complete
        for (auto& thread : player_threads) {
            thread.join();
        }
        
        displayGameResults();
    }

    int selectNextPlayer() {
        // Implement random player selection logic
        std::uniform_int_distribution<> dis(0, NUM_PLAYERS - 1);
        return dis(gen);
    }

    bool isGameOver() {
        // Check if game is complete
        int completed_players = std::count(player_completed.begin(), player_completed.end(), true);
        return completed_players >= NUM_PLAYERS - 1;
    }

    void displayGameResults() {
        std::cout << "Game Results:" << std::endl;
        // Display winners, hit rates, etc.
    }
};

int main() {
    LudoGame game;
    
    int tokens_per_player;
    std::cout << "Enter number of tokens per player (1-4): ";
    std::cin >> tokens_per_player;
    
    if (tokens_per_player < 1 || tokens_per_player > 4) {
        std::cerr << "Invalid number of tokens!" << std::endl;
        return 1;
    }
    
    game.startGame(tokens_per_player);
    
    return 0;
}