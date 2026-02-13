#include "include/Banker.h"
#include <iomanip>
#include <iostream>
#include <sstream> 
#include <thread>
#include <chrono>

using namespace std;

Banker::Banker(const Resources& initialResources, 
               const vector<ClientConfig>& clients,
               const vector<Resources>& initialAllocations) 
    : available_(initialResources), clientCount_(clients.size()) 
{
    resourceCount_ = initialResources.size();
    allocation_ = initialAllocations; 

    for (int i = 0; i < clientCount_; ++i) {
        max_.push_back(clients[i].maxDemand);
        need_.push_back(subtract(clients[i].maxDemand, allocation_[i]));
    }
}

string formatVector(const Resources& r) {
    stringstream ss;
    ss << "[";
    for (size_t i = 0; i < r.size(); ++i) {
        ss << r[i] << (i < r.size() - 1 ? " " : "");
    }
    ss << "]";
    return ss.str();
}

void Banker::printResInner(const Resources& r) const {
    cout << formatVector(r);
}

void Banker::printState(const vector<bool>& finished) {
    cout << "\n   ID  | Allocated       | Max             | Need            | Status\n";
    cout << "-------|-----------------|-----------------|-----------------|--------\n";
    for (int i = 0; i < clientCount_; ++i) {
        cout << "   P" << i << "  | " 
             << left << setw(16) << formatVector(allocation_[i]) 
             << left << setw(16) << formatVector(max_[i]) 
             << left << setw(16) << formatVector(need_[i]);
        
        if (finished[i]) cout << " [DONE]";
        else cout << " [WAIT]";
        cout << "\n";
    }
    cout << "-------|-----------------|-----------------|-----------------|--------\n";
    cout << "   Available: " << formatVector(available_) << "\n\n";
}

void Banker::rollbackResources(int clientId, const Resources& request) {
    allocation_[clientId] = subtract(allocation_[clientId], request);
    available_ = add(available_, request);      
    need_[clientId] = add(need_[clientId], request); 

    cout << "   <<< ROLLBACK: Откат ресурсов для P" << clientId << " >>>\n";
}

void Banker::runSafeSequenceSimulation() {
    vector<bool> finish(clientCount_, false);
    int finishedCount = 0;
    
    cout << "=== НАЧАЛО СИМУЛЯЦИИ (Safe Sequence) ===\n";
    printState(finish);

    while (finishedCount < clientCount_) {
        bool progressMade = false;

        for (int i = 0; i < clientCount_; ++i) {
            if (!finish[i]) {
                cout << "Проверяем P" << i << "... Need: " << formatVector(need_[i]);

                if (lessOrEqual(need_[i], available_)) {
                    cout << " <= Available " << formatVector(available_) << ". ХВАТАЕТ!\n";

                    available_ = subtract(available_, need_[i]);
                    allocation_[i] = add(allocation_[i], need_[i]);
                    
                    cout << "   -> Выдали ресурсы P" << i << "...\n";
                    this_thread::sleep_for(chrono::milliseconds(800));

                    available_ = add(available_, allocation_[i]);
                    
                    finish[i] = true;
                    finishedCount++;
                    progressMade = true;

                    cout << "   -> P" << i << " завершился. Available вырос: " << formatVector(available_) << "\n";
                    printState(finish);
                    
                } else {
                    cout << " > Available " << formatVector(available_) << ". МАЛО -> Ждем.\n";
                }
                
                this_thread::sleep_for(chrono::milliseconds(300));
            }
        }

        if (!progressMade) {
            cout << "\n!!! DEADLOCK (ТУПИК) !!!\n";
            return;
        }
    }

    cout << "=== УСПЕХ! Все процессы завершены. ===\n";
}

bool Banker::lessOrEqual(const Resources& a, const Resources& b) const {
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] > b[i]) return false;
    }
    return true;
}

Resources Banker::subtract(const Resources& a, const Resources& b) const {
    Resources res = a; 
    for (size_t i = 0; i < a.size(); ++i) {
        res[i] -= b[i];
    }
    return res;
}

Resources Banker::add(const Resources& a, const Resources& b) const {
    Resources res = a; 
    for (size_t i = 0; i < a.size(); ++i) {
        res[i] += b[i]; 
    }
    return res;
}