#pragma once

#include <iostream>
#include <iomanip>

using namespace std;

// Структура даты (день, месяц, год)
struct Date {
    int day;
    int month;
    int year;

    // Перегрузка оператора "меньше" для сравнения дат
    bool operator<(const Date& other) const {
        if (year != other.year) return year < other.year;
        if (month != other.month) return month < other.month;
        return day < other.day;
    }

    // Перегрузка оператора "больше или равно"
    bool operator>=(const Date& other) const {
        return !(*this < other);
    }

    // Перегрузка оператора "меньше или равно"
    bool operator<=(const Date& other) const {
        return (*this < other) || (*this == other);
    }

    // Оператор равенства
    bool operator==(const Date& other) const {
        return day == other.day && month == other.month && year == other.year;
    }

    // Удобный вывод даты в поток (например, cout)
    friend ostream& operator<<(ostream& os, const Date& d) {
        os << setfill('0') << setw(2) << d.day << "."
           << setw(2) << d.month << "." << d.year;
        return os;
    }
};
