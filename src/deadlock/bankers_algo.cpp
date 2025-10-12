#include "bankers_algo.h"
#include <iostream>
using namespace std;
BankersAlgorithm::BankersAlgorithm(int processes, int resources) 
    : num_processes(processes), num_resources(resources) 
{
    
    available = vector<int>(num_resources, 1000); //initial_balance 1000
    
    //Initializing matrix
    max = vector<vector<int>>(num_processes, vector<int>(num_resources, 500));
    allocation = vector<vector<int>>(num_processes, vector<int>(num_resources, 0));
    need = vector<vector<int>>(num_processes, vector<int>(num_resources, 500));
}
bool BankersAlgorithm::is_safe_state() 
{
    vector<int> work = available;
    vector<bool> finish(num_processes, false);
    vector<int> safe_sequence;
    int count = 0;
    bool found;
    do {
        found = false;
        for (int i = 0; i < num_processes; i++) {
            if (!finish[i]) {
                //Checking for process i
                bool can_allocate = true;
                for (int j = 0; j < num_resources; j++) {
                    if (need[i][j] > work[j]) {
                        can_allocate = false;
                        break;
                    }
                }
                
                if (can_allocate) {
                    for (int j = 0; j < num_resources; j++) {
                        work[j] += allocation[i][j];
                    }
                    safe_sequence.push_back(i);
                    finish[i] = true;
                    found = true;
                    count++;
                }
            }
        }
    } while (found);
    
    // If all processes finished, system is in safe state
    if (count == num_processes) {
        cout << "System is in SAFE state" << endl;
        cout << "Safe sequence: ";
        for (int i : safe_sequence) {
            cout << "P" << i << " ";
        }
        cout << endl;
        return true;
    } else {
        cout << "System is in UNSAFE state - Deadlock possible!" << endl;
        return false;
    }
}

bool BankersAlgorithm::request_resources(int process_id, vector<int> request) {
    cout << "Process P" << process_id << " requesting: ";
    for (int r : request) cout << r << " ";
    cout << endl;
    for (int i = 0; i < num_resources; i++) {
        if (request[i] > need[process_id][i]) {
            cout << "Error: Request exceeds maximum claim!" << endl;
            return false;
        }
    }
    for (int i = 0; i < num_resources; i++) {
        if (request[i] > available[i]) {
            cout << "Process must wait - resources not available" << endl;
            return false;
        }
    }
    for (int i = 0; i < num_resources; i++) {
        available[i] -= request[i];
        allocation[process_id][i] += request[i];
        need[process_id][i] -= request[i];
    }
    //Safety check
    if (is_safe_state()) {
        cout << "Request GRANTED - system remains in safe state" << endl;
        return true;
    } else {
        
        //  Rollback if unsafe
        cout << "Rolling back allocation - would lead to unsafe state" << endl;
        for (int i = 0; i < num_resources; i++) {
            available[i] += request[i];
            allocation[process_id][i] -= request[i];
            need[process_id][i] += request[i];
        }
        cout << "Request DENIED" << endl;
        return false;
    }
}

void BankersAlgorithm::print_system_state() {
    cout << "Available resources: ";
    for (int a : available) cout << a << " ";
    cout << endl;
    
    cout << "\nMax Matrix (Maximum demand):" << endl;
    for (int i = 0; i < num_processes; i++) {
        cout << "P" << i << ": ";
        for (int j = 0; j < num_resources; j++) {
            cout << max[i][j] << " ";
        }
        cout << endl;
    }
    
    cout << "\nAllocation Matrix (Currently allocated):" << endl;
    for (int i = 0; i < num_processes; i++) {
        cout << "P" << i << ": ";
        for (int j = 0; j < num_resources; j++) {
            cout << allocation[i][j] << " ";
        }
        cout << endl;
    }
    
    cout << "\nNeed Matrix (Remaining need):" << endl;
    for (int i = 0; i < num_processes; i++) {
        cout << "P" << i << ": ";
        for (int j = 0; j < num_resources; j++) {
            cout << need[i][j] << " ";
        }
        cout << endl;
    }
}