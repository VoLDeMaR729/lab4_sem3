#pragma once

#include <vector>
#include <iostream>

using namespace std;

using Resources = vector<int>;

struct ClientConfig {
    int id;
    Resources maxDemand;
};