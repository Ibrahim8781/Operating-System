#ifndef LUDO_GAME_H
#define LUDO_GAME_H

#include <iostream>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <random>
#include <algorithm>
#include <iomanip>

const int BOARD_SIZE = 52;
const int NUM_PLAYERS = 4;
const int MAX_TOKENS_PER_PLAYER = 4;
const int HOME_COLUMN_START = 51;

// Safe squares for each color
const std::vector<int> SAFE_SQUARES = {5, 12, 17, 22, 29, 34, 39, 46};

enum PlayerColor {
    RED, GREEN, BLUE, YELLOW
};

struct Token {
    int position;
    bool in_play;
    bool in_home;

    Token() : position(-1), in_play(false), in_home(false) {}
};

struct Player {
    int id;
    PlayerColor color;
    std::vector<Token> tokens;
    int hit_rate;
    bool is_active;
    int consecutive_six_count;

    Player() : id(-1), color(RED), tokens(MAX_TOKENS_PER_PLAYER), 
               hit_rate(0), is_active(true), consecutive_six_count(0) {}
};

class LudoGame {
private:
    std::vector<Player> players;
    std::vector<int> board;
    std::mutex dice_mutex;
    std::mutex board_mutex;
    std::condition_variable turn_cv;
    
    std::mt19937 random_generator;
    std::uniform_int_distribution<> dice_distribution;

    int current_turn;
    int last_player_turn;
    int dice_value;
    
    std::vector<Player> game_rankings;

    // Private helper methods
    bool is_safe_square(int position, PlayerColor color);
    int get_token_start_position(PlayerColor color);
    void reset_game();
    int roll_dice();
    bool can_move_token(Player& player, Token& token, int dice_roll);
    void move_token(Player& player, Token& token, int dice_roll);
    void hit_token(int position);
    bool check_win_condition(Player& player);
    void check_player_elimination();
    int get_home_column_position(PlayerColor color, int relative_pos);

public:
    LudoGame();
    void initialize_game(const std::vector<int>& tokens_per_player);
    void play_game();
    void display_board();
    void display_results();
    bool play_turn(int player_id);
    bool is_game_over();
};

#endif