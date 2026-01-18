#pragma once
#include "Banker.h"
#include <thread>
#include <atomic>

class Client {
    ClientConfig config_;
    Banker& banker_;
    thread thread_;
    atomic<bool> running_;

    void lifeCycle(); // Основной цикл жизни клиента

public:
    Client(const ClientConfig& cfg, Banker& banker);
    ~Client();

    void start();
    void stop();
    void join();
};
