#include "ludo_game.h"
#include "game_thread.h"

int main() {
    LudoGame game;
    
    // Get user input for number of tokens per player
    std::vector<int> tokens_per_player(NUM_PLAYERS);
    for (int i = 0; i < NUM_PLAYERS; ++i) {
        do {
            std::cout << "Enter number of tokens for Player " << i + 1 
                      << " (1-4): ";
            std::cin >> tokens_per_player[i];
        } while (tokens_per_player[i] < 1 || tokens_per_player[i] > 4);
    }

    game.initialize_game(tokens_per_player);

    // Start the game
    game.play_game();

    return 0;
}