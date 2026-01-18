#include "include/DataGenerator.h"

// Получение количества дней в месяце
int DataGenerator::getMaxDays(int month, int year) const {
    if (month == 2) {
        // Проверка на високосный год
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        return isLeap ? 29 : 28;
    }
    // Месяцы с 30 днями: Апрель, Июнь, Сентябрь, Ноябрь
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    }
    return 31;
}

// Генерация массива дат
vector<Date> DataGenerator::generateDates(size_t count, int startYear, int endYear) const {
    vector<Date> result;
    result.reserve(count); // Заранее выделяем память для скорости

    uniform_int_distribution<> yearDist(startYear, endYear);
    uniform_int_distribution<> monthDist(1, 12);

    for (size_t i = 0; i < count; ++i) {
        Date d;
        d.year = yearDist(gen);
        d.month = monthDist(gen);
        
        // Генерируем день корректно для выпавшего месяца
        int maxDay = getMaxDays(d.month, d.year);
        uniform_int_distribution<> dayDist(1, maxDay);
        d.day = dayDist(gen);

        result.push_back(d);
    }

    return result;
}
