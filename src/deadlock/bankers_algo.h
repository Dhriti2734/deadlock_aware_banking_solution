#ifndef BANKERS_ALGORITHM_H
#define BANKERS_ALGORITHM_H

#include <vector>
#include <iostream>
using namespace std;

class BankersAlgorithm 
{
    private:vector<int> available;
    vector<vector<int>> max;
    vector<vector<int>> allocation;
    vector<vector<int>> need;
    int num_processes;
    int num_resources;
    
    public: BankersAlgorithm(int processes, int resources);
    bool request_resources(int process_id, vector<int> request);
    bool is_safe_state();
    void print_system_state();
};

#endif