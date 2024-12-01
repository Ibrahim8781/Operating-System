#include "ludo_game.h"
#include <thread>
#include <chrono>

LudoGame::LudoGame() : 
    board(BOARD_SIZE, -1), 
    random_generator(std::chrono::steady_clock::now().time_since_epoch().count()),
    dice_distribution(1, 6),
    current_turn(0), 
    last_player_turn(-1),
    dice_value(0)
{
    // Initialize players
    for (int i = 0; i < NUM_PLAYERS; ++i) {
        players[i].id = i;
        players[i].color = static_cast<PlayerColor>(i);
    }
}

void LudoGame::initialize_game(const std::vector<int>& tokens_per_player) {
    for (int i = 0; i < NUM_PLAYERS; ++i) {
        int num_tokens = std::min(tokens_per_player[i], MAX_TOKENS_PER_PLAYER);
        players[i].tokens.resize(num_tokens);
        for (auto& token : players[i].tokens) {
            token = Token();
        }
    }
}

int LudoGame::roll_dice() {
    std::lock_guard<std::mutex> lock(dice_mutex);
    return dice_distribution(random_generator);
}

bool LudoGame::is_safe_square(int position, PlayerColor color) {
    // Check if the position is a global safe square
    auto it = std::find(SAFE_SQUARES.begin(), SAFE_SQUARES.end(), position);
    if (it != SAFE_SQUARES.end()) return true;
    
    // Add color-specific home path safety logic
    return false;
}

void LudoGame::move_token(Player& player, Token& token, int dice_roll) {
    std::lock_guard<std::mutex> lock(board_mutex);
    
    // Check if token is entering the game
    if (!token.in_play && dice_roll == 6) {
        token.position = get_token_start_position(player.color);
        token.in_play = true;
        return;
    }

    // Move token if already in play
    if (token.in_play) {
        int new_position = (token.position + dice_roll) % BOARD_SIZE;
        
        // Check for hitting opponent tokens
        if (board[new_position] != -1 && board[new_position] != player.id) {
            hit_token(new_position);
            player.hit_rate++;
        }

        token.position = new_position;

        // Check if token reaches home column
        if (new_position >= HOME_COLUMN_START) {
            token.in_home = true;
        }
    }
}

void LudoGame::hit_token(int position) {
    // Find the token at the position and send it back to its yard
    for (auto& player : players) {
        for (auto& token : player.tokens) {
            if (token.position == position) {
                token.position = -1;
                token.in_play = false;
                break;
            }
        }
    }
}

bool LudoGame::can_move_token(Player& player, Token& token, int dice_roll) {
    // Implement detailed move validation
    if (!token.in_play && dice_roll != 6) return false;
    if (token.in_home) return false;
    
    int new_position = (token.position + dice_roll) % BOARD_SIZE;
    
    // Check for blocking by own color tokens
    for (const auto& other_token : player.tokens) {
        if (other_token.position == new_position) return false;
    }
    
    return true;
}

bool LudoGame::play_turn(int player_id) {
    Player& current_player = players[player_id];
    
    // Check if player is active
    if (!current_player.is_active) return false;
    
    int dice_roll = roll_dice();
    bool moved = false;
    
    // Handle consecutive six rolls
    if (dice_roll == 6) {
        current_player.consecutive_six_count++;
        if (current_player.consecutive_six_count >= 3) {
            current_player.consecutive_six_count = 0;
            return false;  // Skip turn
        }
    } else {
        current_player.consecutive_six_count = 0;
    }
    
    // Try to move a token
    for (auto& token : current_player.tokens) {
        if (can_move_token(current_player, token, dice_roll)) {
            move_token(current_player, token, dice_roll);
            moved = true;
            break;
        }
    }
    
    // Check win condition
    if (check_win_condition(current_player)) {
        game_rankings.push_back(current_player);
        current_player.is_active = false;
    }
    
    return moved;
}

void LudoGame::play_game() {
    while (!is_game_over()) {
        // Random turn selection
        std::uniform_int_distribution<> player_dist(0, NUM_PLAYERS - 1);
        int current_player = player_dist(random_generator);
        
        play_turn(current_player);
        
        // Delay to simulate turn and allow board visualization
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        
        display_board();
    }
    
    display_results();
}

bool LudoGame::is_game_over() {
    int active_players = std::count_if(players.begin(), players.end(), 
        [](const Player& p) { return p.is_active; });
    return active_players <= 1;
}

void LudoGame::display_board() {
    std::cout << "Current Game Board:\n";
    for (const auto& player : players) {
        std::cout << "Player " << player.id << " (Color: " 
                  << player.color << ") Tokens:\n";
        for (const auto& token : player.tokens) {
            std::cout << "Position: " << token.position 
                      << ", In Play: " << token.in_play 
                      << ", In Home: " << token.in_home << "\n";
        }
    }
    std::cout << "\n";
}

void LudoGame::display_results() {
    std::cout << "Game Results:\n";
    for (size_t i = 0; i < game_rankings.size(); ++i) {
        std::cout << (i + 1) << " Place: Player " 
                  << game_rankings[i].id << "\n";
    }
}

bool LudoGame::check_win_condition(Player& player) {
    return std::all_of(player.tokens.begin(), player.tokens.end(), 
        [](const Token& t) { return t.in_home; });
}

int LudoGame::get_token_start_position(PlayerColor color) {
    switch (color) {
        case RED: return 0;
        case GREEN: return 13;
        case BLUE: return 26;
        case YELLOW: return 39;
        default: return -1;
    }
}
