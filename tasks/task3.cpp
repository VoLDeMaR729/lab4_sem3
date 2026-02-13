#include "../BankerLib/include/Banker.h"
#include <vector>
#include <iostream>

using namespace std;

int main() {
    Resources bankResources = {3, 3, 2}; 

    vector<Resources> maxDemand = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3} 
    };

    vector<Resources> allocation = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2} 
    };

    vector<ClientConfig> clientsConf;
    for(size_t i = 0; i < maxDemand.size(); ++i) {
        clientsConf.push_back({static_cast<int>(i), maxDemand[i]});
    }

    Banker banker(bankResources, clientsConf, allocation);
    banker.runSafeSequenceSimulation();

    return 0;
}