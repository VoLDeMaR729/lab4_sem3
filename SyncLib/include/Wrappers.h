#pragma once

#include <thread>
#include <mutex>
#include <semaphore>
#include <barrier>
#include <atomic>
#include <condition_variable>

using namespace std;

// Интерфейс (абстрактный класс) для примитивов синхронизации
class ILock {
public:
    virtual ~ILock() = default;
    virtual void lock() = 0;   // Захват ресурса
    virtual void unlock() = 0; // Освобождение ресурса
};

// Обертка над стандартным мьютексом (std::mutex)
class MutexImpl : public ILock {
    mutex m_; 
public:
    void lock() override;
    void unlock() override;
};

// Обертка над бинарным семафором (std::counting_semaphore)
class SemaImpl : public ILock {
    counting_semaphore<1> sem_; // Семафор с счетчиком 1 (аналог мьютекса)
public:
    SemaImpl(): sem_(1) {}
    void lock() override;
    void unlock() override;
};

// Обертка над барьером
class BarrImpl {
     barrier<>* bar_; // Указатель на барьер
public:
    // Конструктор принимает количество ожидаемых потоков
    BarrImpl(int th_count) {
        bar_ = new barrier(th_count);
    }
    
    // Ожидание остальных потоков
    void wait();
};

// Спин-блокировка на основе atomic_flag
class SpinL : public ILock {
    atomic_flag f_;
public:
    SpinL(): f_(ATOMIC_FLAG_INIT) {}
    void lock() override;
    void unlock() override;
};

// Спин-ожидание (SpinWait) с уступкой процессора
class SpinW : public ILock {
    atomic_flag f_;
public:
    SpinW(): f_{} {}
    void lock() override;
    void unlock() override;
};

// Монитор (реализация через мьютекс и условную переменную)
class MonImpl : public ILock {
    mutex m_;
    condition_variable cv_;
    bool is_busy; // Флаг занятости монитора

public:
    MonImpl(): is_busy(false) {}
    void lock() override;
    void unlock() override;
};
