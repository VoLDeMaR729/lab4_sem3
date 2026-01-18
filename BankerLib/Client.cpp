#include "include/Client.h"
#include <chrono>
#include <random>

//Конструктор: инициализация конфигурации и ссылки на Банкира
Client::Client(const ClientConfig& cfg, Banker& banker)
    : config_(cfg), banker_(banker), running_(false) {}

//Деструктор: остановка потока перед уничтожением объекта
Client::~Client() {
    stop();
    join();
}

//Запуск потока клиента
void Client::start() {
    running_ = true;
    thread_ = thread(&Client::lifeCycle, this);
}

//Установка флага остановки
void Client::stop() {
    running_ = false;
}

//Ожидание завершения потока
void Client::join() {
    if (thread_.joinable()) thread_.join();
}

//Жизненный цикл: Думает -> Просит -> Тратит -> Возвращает
void Client::lifeCycle() {
    random_device rd;
    mt19937 gen(rd());
    
    while (running_) {
        //Имитация работы без ресурсов (думает)
        this_thread::sleep_for(chrono::milliseconds(500 + gen() % 500));

//Формируем случайный запрос ресурсов (часть от максимального)
        Resources request = config_.maxDemand;
        for(size_t i=0; i<request.size(); ++i) {
            //Просим от 0 до Max[i]
            if (request[i] > 0) request[i] = (gen() % request[i]) + 1;
        }

//Попытка взять кредит (цикл пока не дадут)
        bool success = false;
        while (running_ && !success) {
            success = banker_.requestResources(config_.id, request);
            if (!success) {
                //Если отказали — ждем и пробуем снова
                this_thread::sleep_for(chrono::milliseconds(100));
            }
        }

        if (!running_) break;

        //Использование ресурсов
        this_thread::sleep_for(chrono::milliseconds(1000 + gen() % 1000));

        //Возврат ресурсов
        banker_.releaseResources(config_.id, request);
    }
}
