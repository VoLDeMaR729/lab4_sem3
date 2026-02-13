#pragma once

#include "BankerStructs.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

class Banker {
    Resources available_;
    vector<Resources> max_;        
    vector<Resources> allocation_; 
    vector<Resources> need_;       
    
    int clientCount_;
    int resourceCount_;

public:
    Banker(const Resources& initialResources, 
           const vector<ClientConfig>& clients,
           const vector<Resources>& initialAllocations);

    void runSafeSequenceSimulation();
    void rollbackResources(int clientId, const Resources& request);

private:
    bool lessOrEqual(const Resources& a, const Resources& b) const;
    Resources add(const Resources& a, const Resources& b) const;
    Resources subtract(const Resources& a, const Resources& b) const;
    
    void printResInner(const Resources& r) const;
    void printState(const vector<bool>& finished);
};