#include "game_thread.h"

GameThread::GameThread(int id, LudoGame* game_ref) : 
    thread_id(id), game(game_ref) {}

void GameThread::start() {
    worker_thread = std::thread(&GameThread::thread_worker, this);
}

void GameThread::stop() {
    if (worker_thread.joinable()) {
        worker_thread.join();
    }
}

void GameThread::thread_worker() {
    // Implement thread-specific game logic
}
