#pragma once
#include <vector>
#include <iostream>

using namespace std;

//Тип ресурса Например: [3, 2, 2] означает 3 принтера, 2 сканера, 2 диска
using Resources = vector<int>;

// Конфигурация клиента
struct ClientConfig {
    int id;
    Resources maxDemand; // Максимальная потребность клиента
};

// Вспомогательная функция для печати ресурсов
inline void printRes(const Resources& r) {
    cout << "[ ";
    for (int val : r) cout << val << " ";
    cout << "]";
}
