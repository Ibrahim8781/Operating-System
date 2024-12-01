#include "ludo_game.h"
#include "game_thread.h"
#include <vector>
#include <iostream>

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

    // Create game threads
    std::vector<GameThread> threads;
    for (int i = 0; i < NUM_PLAYERS; ++i) {
        threads.emplace_back(i, &game);
        threads[i].start();
    }

    game.play_game();

    // Stop threads
    for (auto& thread : threads) {
        thread.stop();
    }

    game.display_results();

    return 0;
}