#ifndef GAME_THREAD_H
#define GAME_THREAD_H

#include "ludo_game.h"
#include <thread>

class GameThread {
private:
    std::thread worker_thread;
    int thread_id;
    LudoGame* game;

public:
    GameThread(int id, LudoGame* game_ref);
    void start();
    void stop();
    void thread_worker();
};

#endif
