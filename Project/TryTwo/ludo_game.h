#ifndef LUDO_GAME_H
#define LUDO_GAME_H

#include <iostream>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <random>
#include <algorithm>

const int BOARD_SIZE = 52;
const int NUM_PLAYERS = 4;
const int MAX_TOKENS_PER_PLAYER = 4;

enum PlayerColor {
    RED, GREEN, BLUE, YELLOW
};

struct Token {
    int position;
    bool in_play;
    bool in_home;
};

struct Player {
    int id;
    PlayerColor color;
    std::vector<Token> tokens;
    int hit_rate;
    bool is_active;
};

class LudoGame {
private:
    std::vector<Player> players;
    std::vector<int> board;
    std::mutex dice_mutex;
    std::mutex board_mutex;
    std::condition_variable turn_cv;
    int current_turn;
    int last_player_turn;
    int dice_value;
    std::mt19937 random_generator;

    // Private helper methods
    bool is_safe_square(int position, PlayerColor color);
    void reset_game();
    int roll_dice();
    bool can_move_token(Player& player, Token& token, int dice_roll);
    void move_token(Player& player, Token& token, int dice_roll);
    void hit_token(int position);
    bool check_win_condition(Player& player);

public:
    LudoGame();
    void initialize_game(const std::vector<int>& tokens_per_player);
    void play_game();
    void display_board();
    void display_results();
};

#endif

