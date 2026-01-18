#include "../DataLib/include/DataGenerator.h"
#include "../DataLib/include/DateProcessor.h"
#include "../SyncLib/include/Timer.h" // Используем таймер из 1-го задания
#include <iostream>

using namespace std;

// Функция для ввода даты с клавиатуры
Date inputDate(string prompt) {
    Date d;
    cout << prompt << " (День Месяц Год через пробел): ";
    cin >> d.day >> d.month >> d.year;
    return d;
}

int main() {
    size_t dataSize;
    int threadCount;

    // Ввод параметров
    cout << "=== Задание 2: Обработка дат ===\n";
    cout << "Введите размер массива данных (например, 10000000): ";
    cin >> dataSize;
    cout << "Введите количество потоков: ";
    cin >> threadCount;

    cout << "\n--- Задание диапазона поиска ---\n";
    Date d1 = inputDate("Введите начальную дату D1");
    Date d2 = inputDate("Введите конечную дату D2");

    // Генерация данных
    cout << "\nГенерация данных..." << endl;
    DataGenerator gen;
    // Генерируем массив
    auto dates = gen.generateDates(dataSize); 
    
    DateProcessor processor(dates);
    Timer timer;

    cout << "Данные сгенерированы. Начинаем обработку.\n" << endl;

    // 1. Последовательный запуск
    timer.start();
    int resSeq = processor.processSequence(d1, d2);
    long long timeSeq = timer.stop();

    // 2. Параллельный запуск
    timer.start();
    int resPar = processor.processParallel(d1, d2, threadCount);
    long long timePar = timer.stop();

    // Вывод результатов
    cout << "--------------------------------" << endl;
    cout << "Результат (Последовательно): найдено " << resSeq << " дат" << endl;
    cout << "Время: " << timeSeq << " мс" << endl;
    cout << "--------------------------------" << endl;
    cout << "Результат (Параллельно):     найдено " << resPar << " дат" << endl;
    cout << "Время: " << timePar << " мс" << endl;
    
    // Проверка корректности (результаты должны совпадать)
    if (resSeq == resPar) {
        cout << "\n[OK] Результаты совпадают." << endl;
    } else {
        cout << "\n[ERROR] Результаты не совпадают!" << endl;
    }

    return 0;
}
