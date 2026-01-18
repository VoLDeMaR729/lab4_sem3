#pragma once

#include "DateStruct.h"
#include <vector>
#include <random>

// Класс для генерации массива случайных дат
class DataGenerator {
    // Генератор случайных чисел
    mutable random_device rd;
    mutable mt19937 gen;

public:
    DataGenerator() : gen(rd()) {}

//Метод генерации заданного количества дат, задают границы генерации
    vector<Date> generateDates(size_t count, int startYear = 2000, int endYear = 2030) const;

private:
    // Вспомогательный метод валидации дня (учитывает високосные годы)
    int getMaxDays(int month, int year) const;
};
