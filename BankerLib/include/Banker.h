#pragma once

#include "BankerStructs.h"
#include <vector>
#include <mutex>

class Banker {
    Resources available_; // Доступные ресурсы в банке (Available)
    
// Матрицы состояния системы
// Индекс вектора = ID клиента
    vector<Resources> max_;//Максимальная потребность (Max)
    vector<Resources> allocation_;//Текущие выделенные ресурсы (Allocation)
    vector<Resources> need_;//Сколько еще нужно (Need = Max - Allocation)

    int clientCount_;
    int resourceCount_;
    mutable mutex mtx_; // Мьютекс для защиты состояния банка

public:
// Конструктор: принимает начальные ресурсы банка и конфиги всех клиентов
    Banker(const Resources& initialResources, const vector<ClientConfig>& clients);

// Попытка запросить ресурсы
// Возвращает true, если запрос удовлетворен (состояние безопасно)
//Возвращает false, если запрос отклонен (небезопасно или нет ресурсов)
    bool requestResources(int clientId, const Resources& request);

    // Возврат ресурсов обратно в банк
    void releaseResources(int clientId, const Resources& release);

    // Проверка состояния системы (вывод инфо)
    void printState();

private:
//Проверка на безопасность
//Проверяет, существует ли последовательность выполнения процессов,при которой не возникнет Deadlock
    bool isSafeState(const Resources& currentAvail, 
                     const vector<Resources>& currentAlloc, 
                     const vector<Resources>& currentNeed);

    // Вспомогательные операции с векторами
    bool lessOrEqual(const Resources& a, const Resources& b) const;
    Resources subtract(const Resources& a, const Resources& b) const;
    Resources add(const Resources& a, const Resources& b) const;
};
