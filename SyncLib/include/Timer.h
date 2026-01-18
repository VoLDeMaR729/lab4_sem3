#pragma once

#include <chrono>

using namespace std::chrono;

// Класс таймера для замера времени выполнения блоков кода
class Timer {
    time_point<high_resolution_clock> t_start; // Время начала
    bool active; // Флаг состояния таймера

public:
    Timer() : active(false) {}

    // Запуск отсчета времени
    void start() {
        t_start = high_resolution_clock::now(); // Фиксируем текущее время
        active = true;
    }

    // Остановка таймера и получение результата в миллисекундах
    long long stop() {
        if (!active) return 0; // Если таймер не был запущен

        auto t_end = high_resolution_clock::now(); // Фиксируем время окончания
        active = false;

        // Возвращаем разницу в миллисекундах
        return duration_cast<milliseconds>(t_end - t_start).count();
    }
};
