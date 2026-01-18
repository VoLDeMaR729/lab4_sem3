#include "include/Wrappers.h"

// Реализация методов мьютекса
void MutexImpl::lock() {
    m_.lock(); // Блокируем поток
}

void MutexImpl::unlock() {
    m_.unlock(); // Разблокируем
}

// Реализация методов семафора
void SemaImpl::lock() {
    sem_.acquire(); // Уменьшаем счетчик (ждем если 0)
}

void SemaImpl::unlock() {
    sem_.release(); // Увеличиваем счетчик
}


// Реализация барьера
void BarrImpl::wait() {
    bar_->arrive_and_wait(); // Поток прибыл и ждет остальных
}


// Реализация SpinLock (активное ожидание)
void SpinL::lock() {
    // Крутимся в цикле пока флаг занят (test_and_set возвращает true если было установлено)
    while (f_.test_and_set(memory_order_acquire)) {}
}

void SpinL::unlock() {
    f_.clear(memory_order_release); // Сброс флага
}


// Реализация SpinWait (ожидание с yield)
void SpinW::lock() {
    // Если занято - уступаем квант времени другим потокам
    while (f_.test_and_set(memory_order_acquire)) {
        this_thread::yield(); 
    }
}

void SpinW::unlock() {
    f_.clear(memory_order_release);
}


// Реализация Монитора
void MonImpl::lock() {
    unique_lock<mutex> lk(m_); // Захват мьютекса для работы с условием
    
    // Ждем пока is_busy станет false
    cv_.wait(lk, [this](){ return !is_busy; });
    
    is_busy = true; // Занимаем монитор
}

void MonImpl::unlock() {
    {
        lock_guard<mutex> lk(m_);
        is_busy = false; // Освобождаем
    }
    cv_.notify_one(); // Будим один из ожидающих потоков
}
