#include "include/Banker.h"

// Конструктор: инициализирует ресурсы банка и создает записи для клиентов
Banker::Banker(const Resources& initialResources, const vector<ClientConfig>& clients) 
    : available_(initialResources), clientCount_(clients.size()) 
{
    resourceCount_ = initialResources.size();
    
    // Проход по всем клиентам для настройки начального состояния
    for (const auto& cli : clients) {
        max_.push_back(cli.maxDemand); // Запоминаем максимальные потребности
        allocation_.push_back(Resources(resourceCount_, 0)); // Изначально у клиента 0 ресурсов
        need_.push_back(cli.maxDemand); // Потребность равна максимуму (так как ничего еще не дали)
    }
}


// Запрос ресурсов: проверяет возможность выдачи и безопасность состояния
bool Banker::requestResources(int clientId, const Resources& request) {
    lock_guard<mutex> lock(mtx_); // Блокируем доступ, чтобы потоки не мешали друг другу

//Проверка логики: нельзя просить больше, чем было заявлено в Max
    if (!lessOrEqual(request, need_[clientId])) {
        return false; 
    }

//Проверка наличия: есть ли сейчас свободные ресурсы в банке
    if (!lessOrEqual(request, available_)) {
        return false;
    }

//Эмуляция: создаем временные копии состояния, будто мы выдали ресурсы
    Resources availCopy = subtract(available_, request);
    vector<Resources> allocCopy = allocation_;
    vector<Resources> needCopy = need_;

    // Обновляем временные матрицы (выдаем ресурсы клиенту)
    allocCopy[clientId] = add(allocCopy[clientId], request);
    needCopy[clientId] = subtract(needCopy[clientId], request);

//Проверка безопасности: запускаем Алгоритм Банкира на временных данных
    if (isSafeState(availCopy, allocCopy, needCopy)) {
        // Состояние безопасное — применяем изменения к реальным данным
        available_ = availCopy;
        allocation_ = allocCopy;
        need_ = needCopy;
        
        cout << " [Banker] Клиент " << clientId << " получил ресурсы: ";
        printRes(request);
        cout << " -> OK" << endl;
        return true;
    } else {
        return false;
    }
}


// Возврат ресурсов: клиент возвращает взятое обратно в банк
void Banker::releaseResources(int clientId, const Resources& release) {
    lock_guard<mutex> lock(mtx_); // Блокировка

    // Забираем ресурсы у клиента (в базе данных банка)
    allocation_[clientId] = subtract(allocation_[clientId], release);
    
// Возвращаем ресурсы в общую кучу
    available_ = add(available_, release);
    
//Увеличиваем потребность
    need_[clientId] = add(need_[clientId], release);

    cout << " [Banker] Клиент " << clientId << " вернул ресурсы:  ";
    printRes(release);
    cout << endl;
}


//Алгоритм безопасности:проверяет, не возникнет ли Deadlock
bool Banker::isSafeState(const Resources& currentAvail, 
                         const vector<Resources>& currentAlloc, 
                         const vector<Resources>& currentNeed) {
    
    Resources work = currentAvail; // Рабочий вектор доступных ресурсов
    vector<bool> finish(clientCount_, false); // Массив, показывающий, завершился ли процесс
    int finishedCount = 0; // Счетчик завершенных процессов

//Пытаемся найти последовательность, в которой все процессы завершатся
    while (finishedCount < clientCount_) {
        bool foundProcess = false;

//Ищем процесс, который может выполниться при текущих ресурсах
        for (int i = 0; i < clientCount_; ++i) {
// Если процесс еще не помечен завершенным И его нужды меньше или равны доступным
            if (!finish[i] && lessOrEqual(currentNeed[i], work)) {
                
//Эмулируем выполнение процесса: он возвращает ресурсы в систему
                work = add(work, currentAlloc[i]);
                
                finish[i] = true;//Помечаем как завершенный
                finishedCount++;
                foundProcess = true;
            }
        }

// Если за полный проход ни один процесс не смог выполниться — это Deadlock
        if (!foundProcess) {
            return false;
        }
    }

    return true;
}

//Вспомогательные функции
//покомпонентное сравнение векторов (a <= b)
bool Banker::lessOrEqual(const Resources& a, const Resources& b) const {
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] > b[i]) return false;//Если хоть один ресурс 'a' больше 'b'
    }
    return true;
}


//покомпонентное вычитание векторов (a - b)
Resources Banker::subtract(const Resources& a, const Resources& b) const {
    Resources res = a;
    for (size_t i = 0; i < a.size(); ++i) res[i] -= b[i];//Вычитаем каждый ресурс
    return res;
}


//покомпонентное сложение векторов (a + b)
Resources Banker::add(const Resources& a, const Resources& b) const {
    Resources res = a;
    for (size_t i = 0; i < a.size(); ++i) res[i] += b[i];//Складываем каждый ресурс
    return res;
}
