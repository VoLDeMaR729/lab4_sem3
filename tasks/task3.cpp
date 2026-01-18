#include "../BankerLib/include/Banker.h"
#include "../BankerLib/include/Client.h"
#include <vector>
#include <iostream>

using namespace std;

int main() {
//Конфигурация ресурсов банка (например: 10 денег, 5 золота, 7 нефти)
    Resources bankResources = {10, 5, 7}; 
    
    //Создание конфигов клиентов, У каждого свои максимальные потребности
    vector<ClientConfig> clientsConf = {
        {0, {7, 5, 3}}, // Клиент 0 (хочет много)
        {1, {3, 2, 2}}, // Клиент 1 (скромный)
        {2, {9, 0, 2}}, // Клиент 2 (рисковый)
        {3, {2, 2, 2}}, // Клиент 3
        {4, {4, 3, 3}}  // Клиент 4
    };

    cout << "=== Запуск Алгоритма Банкира ===" << endl;
    cout << "Ресурсы банка: "; printRes(bankResources); cout << endl;
    
    // Создаем банкира
    Banker banker(bankResources, clientsConf);

    // Создаем клиентов
    vector<unique_ptr<Client>> clients;
    for (const auto& cfg : clientsConf) {
        clients.push_back(make_unique<Client>(cfg, banker));
    }

    // Запускаем потоки
    for (auto& c : clients) {
        c->start();
    }

//Даем системе поработать 10 секунд, В это время в консоли будет война за ресурсы
    this_thread::sleep_for(chrono::seconds(10));

    // Останавливаем
    cout << "\n=== Остановка системы... ===" << endl;
    for (auto& c : clients) {
        c->stop();
    }
    for (auto& c : clients) {
        c->join();
    }

    cout << "Система остановлена корректно." << endl;

    return 0;
}
