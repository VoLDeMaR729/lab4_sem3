#pragma once

#include "DateStruct.h"
#include <vector>

// Класс для обработки массива дат
class DateProcessor {
    const vector<Date>& data_; // Ссылка на исходные данные

public:
    // Конструктор принимает массив данных
    DateProcessor(const vector<Date>& data) : data_(data) {}

//Последовательный поиск дат в диапазоне [d1, d2], возвращает количество найденных дат
    int processSequence(const Date& d1, const Date& d2) const;

// Параллельный поиск дат в диапазоне [d1, d2], количество потоков
    int processParallel(const Date& d1, const Date& d2, int threadCount) const;
};
