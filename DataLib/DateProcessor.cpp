#include "include/DateProcessor.h"
#include <thread>
#include <numeric> // Для accumulate

// Последовательная обработка
int DateProcessor::processSequence(const Date& d1, const Date& d2) const {
    int count = 0;
    for (const auto& date : data_) {
        // Проверяем попадание в диапазон (включительно)
        if (date >= d1 && date <= d2) {
            count++;
        }
    }
    return count;
}

// Параллельная обработка
int DateProcessor::processParallel(const Date& d1, const Date& d2, int threadCount) const {
    if (data_.empty() || threadCount <= 0) return 0;

    // Если данных мало, нет смысла запускать кучу потоков — ограничиваем
    if (data_.size() < (size_t)threadCount) threadCount = (int)data_.size();

    vector<thread> threads;
    vector<int> results(threadCount, 0); // Вектор для хранения результатов каждого потока
    
    size_t totalSize = data_.size();
    size_t chunkSize = totalSize / threadCount; // Размер куска данных для одного потока

    // Лямбда-функция для работы потока
    auto worker = [&](size_t start, size_t end, int threadIdx) {
        int localCount = 0;
        for (size_t i = start; i < end; ++i) {
            if (data_[i] >= d1 && data_[i] <= d2) {
                localCount++;
            }
        }
        results[threadIdx] = localCount; // Записываем результат
    };

    // Запуск потоков
    for (int i = 0; i < threadCount; ++i) {
        size_t start = i * chunkSize;
        // Последний поток берет всё, что осталось (чтобы не потерять "хвостик" от деления)
        size_t end = (i == threadCount - 1) ? totalSize : start + chunkSize;
        
        threads.emplace_back(worker, start, end, i);
    }

    // Ожидание завершения (join)
    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }

    // Суммирование результатов всех потоков
    int totalFound = 0;
    for (int res : results) {
        totalFound += res;
    }

    return totalFound;
}
