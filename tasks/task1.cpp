#include "../SyncLib/include/Wrappers.h"
#include "../SyncLib/include/Timer.h"
#include <iostream>
#include <random>
#include <vector>

using namespace std;

string g_data; // Общий ресурс (строка), к которому будет доступ из потоков
const int N_THREADS = 12; // Количество потоков
const int N_ITERS = 10000; // Количество операций на один поток

// Рабочая функция потока (добавляет случайные символы в строку)
template<typename TLock>
void worker(TLock& locker) {
    // Настройка генератора случайных чисел
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(32, 126); // Печатные символы ASCII

    for (int i = 0; i < N_ITERS; ++i) {
        // Критическая секция: начало
        locker.lock();
        
        g_data += (char)dist(gen); // Изменение общего ресурса (гонка данных)

        locker.unlock();
        // Критическая секция: конец
    }
}

// Универсальная функция для запуска теста с любым примитивом
template<typename TLock>
long long run_test(const string& label, TLock& locker) {
    g_data.clear(); // Очистка перед новым тестом
    vector<thread> pool;
    Timer tm; // Наш таймер

    tm.start();

    // Создание и запуск потоков
    for (int i = 0; i < N_THREADS; ++i) {
        pool.emplace_back(worker<TLock>, ref(locker));
    }

    // Ожидание завершения всех потоков (join)
    for (auto& t : pool) {
        if (t.joinable()) t.join();
    }

    long long ms = tm.stop(); // Остановка таймера

    // Вывод результатов в консоль
    cout << "[" << label << "]\n";
    cout << "  Time: " << ms << " ms\n";
    cout << "  Res len: " << g_data.length() << "\n\n";

    return ms;
}

// Отдельная функция для теста барьера (логика отличается)
void bar_worker(BarrImpl& bar, atomic<int>& cnt, int iters) {
    for (int i = 0; i < iters; ++i) {
        bar.wait(); // Точка синхронизации: ждем всех
        
        // Безопасный инкремент (atomic)
        cnt.fetch_add(1, memory_order_relaxed);
        
        bar.wait(); // Снова ждем всех перед следующей итерацией
    }
}

// Тест производительности барьера
long long test_barrier(const string& label, BarrImpl& bar) {
    vector<thread> pool;
    atomic<int> cnt{0}; // Атомарный счетчик
    Timer tm;

    tm.start();

    // Запуск потоков
    for (int i = 0; i < N_THREADS; ++i) {
        pool.emplace_back(bar_worker, ref(bar), ref(cnt), N_ITERS);
    }

    // Джоин потоков
    for (auto& t : pool) {
        if (t.joinable()) t.join();
    }

    long long ms = tm.stop();

    cout << "[" << label << "]\n";
    cout << "  Time: " << ms << " ms\n";
    cout << "  Ops: " << cnt.load() << "\n\n";

    return ms;
}

int main() {
    // Вывод заголовка
    cout << "Benchmark (Threads: " << N_THREADS 
         << ", Iters: " << N_ITERS << ")\n";
    cout << "--------------------------------\n";

    // Создание экземпляров примитивов
    MutexImpl m_test;
    SemaImpl s_test;
    SpinL sl_test;
    SpinW sw_test;
    MonImpl mon_test;
    BarrImpl b_test(N_THREADS);

    // Запуск тестов по очереди
    run_test("Mutex", m_test);
    run_test("Semaphore", s_test);
    run_test("SpinLock", sl_test);
    run_test("SpinWait", sw_test);
    run_test("Monitor", mon_test);
    
    // Тест барьера запускается отдельно
    test_barrier("Barrier", b_test);

    return 0;
}
