// main.cpp
// Deadlock Aware Banking Solution - Phase 1
// OS Concepts: Multi-threading, Synchronization, Scheduling, Resource Management

#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>
#include <random>
#include "core/account.h"
#include "scheduling/scheduler.h"
#include "logging/logger.h"
#include "deadlock/deadlock_handler.h"
#include "deadlock/deadlock_demo.h"

using namespace std;

// Global objects for system-wide access
Logger globalLogger("transaction_log.txt");
DeadlockHandler deadlockHandler;

// Thread ID counter
int nextThreadId = 1;

// Helper function to get next thread ID
int getNextThreadId() {
    static mutex idMutex;
    lock_guard<mutex> lock(idMutex);
    return nextThreadId++;
}

// Transaction functions that will be scheduled
void performDeposit(Account& account, double amount) {
    int tid = getNextThreadId();
    cout << "[Thread-" << tid << "] Attempting deposit of $" 
         << amount << " to Account-" << account.getId() << endl;
    
    bool success = account.deposit(amount);
    
    string status = success ? "SUCCESS" : "FAILED";
    globalLogger.logTransaction(tid, "DEPOSIT", account.getId(), amount, status);
    
    cout << "[Thread-" << tid << "] Deposit " << status << endl;
}

void performWithdrawal(Account& account, double amount) {
    int tid = getNextThreadId();
    cout << "[Thread-" << tid << "] Attempting withdrawal of $" 
         << amount << " from Account-" << account.getId() << endl;
    
    bool success = account.withdraw(amount);
    
    string status = success ? "SUCCESS" : "FAILED (Insufficient funds)";
    globalLogger.logTransaction(tid, "WITHDRAW", account.getId(), amount, status);
    
    cout << "[Thread-" << tid << "] Withdrawal " << status << endl;
}

void performTransfer(Account& fromAccount, Account& toAccount, double amount) {
    int tid = getNextThreadId();
    cout << "[Thread-" << tid << "] Attempting transfer of $" << amount 
         << " from Account-" << fromAccount.getId() 
         << " to Account-" << toAccount.getId() << endl;
    
    bool success = fromAccount.transfer(toAccount, amount);
    
    string status = success ? "SUCCESS" : "FAILED (Insufficient funds)";
    globalLogger.logTransfer(tid, fromAccount.getId(), toAccount.getId(), amount, status);
    
    cout << "[Thread-" << tid << "] Transfer " << status << endl;
}

// Display all account balances
void displayAccounts(const vector<unique_ptr<Account>>& accounts) {
    cout << "\n========== Account Status ==========" << endl;
    for (const auto& acc : accounts) {
        cout << acc->toString() << endl;
    }
    cout << "====================================\n" << endl;
}

// Demo function: Run automated test transactions
void runDemoTransactions(vector<unique_ptr<Account>>& accounts, 
                        Scheduler& scheduler) {
    cout << "\n=== Running Demo Transactions ===" << endl;
    
    // Clear any existing tasks
    scheduler.clearTasks();
    
    // Schedule various transactions with different priorities
    // Lower priority number = higher priority
    
    scheduler.addTask(1, 2, "Deposit $1000 to Account-1", 
        [&accounts]() { performDeposit(*accounts[0], 1000.0); });
    
    scheduler.addTask(2, 1, "Withdraw $500 from Account-2", 
        [&accounts]() { performWithdrawal(*accounts[1], 500.0); });
    
    scheduler.addTask(3, 3, "Transfer $300 from Account-1 to Account-3", 
        [&accounts]() { performTransfer(*accounts[0], *accounts[2], 300.0); });
    
    scheduler.addTask(4, 2, "Deposit $750 to Account-3", 
        [&accounts]() { performDeposit(*accounts[2], 750.0); });
    
    scheduler.addTask(5, 1, "Transfer $200 from Account-2 to Account-4", 
        [&accounts]() { performTransfer(*accounts[1], *accounts[3], 200.0); });
    
    // Execute all scheduled tasks
    scheduler.executeTasks();
    
    // Run deadlock detection
    deadlockHandler.detectDeadlock();
    
    cout << "=== Demo Transactions Complete ===\n" << endl;
}

// Display main menu
void displayMenu() {
    cout << "\n========== BANKING SYSTEM MENU ==========" << endl;
    cout << "1. Deposit" << endl;
    cout << "2. Withdraw" << endl;
    cout << "3. Transfer" << endl;
    cout << "4. Show Accounts" << endl;
    cout << "5. Run Demo Transactions" << endl;
    cout << "6. Change Scheduling Policy" << endl;
    cout << "7. Check for Deadlocks\n";
    cout << "8. DEMO: Cause an Intentional Deadlock (WARNING)\n";  
    cout << "9. Exit\n";                                        
    cout << "=============================\n";
    cout << "Enter your choice: ";
}

int main() {
    cout << "=== Deadlock Aware Banking Solution ===" << endl;
    cout << "OS Concepts: Threading, Synchronization, Scheduling" << endl;
    cout << "Target: Windows 11 | Compiler: MinGW GCC 15.2.0 | C++17\n" << endl;

    // OS Concept: Resource initialization
    // Initialize 4 accounts with different balances
    vector<unique_ptr<Account>> accounts;
    accounts.push_back(make_unique<Account>(1, 5000.0));
    accounts.push_back(make_unique<Account>(2, 3000.0));
    accounts.push_back(make_unique<Account>(3, 2000.0));
    accounts.push_back(make_unique<Account>(4, 1500.0));

    cout << "Initialized " << accounts.size() << " accounts." << endl;
    displayAccounts(accounts);

    // Register accounts with deadlock handler
    vector<Account*> accountPtrs;
    for (auto& acc : accounts) {
        accountPtrs.push_back(acc.get());
    }
    deadlockHandler.registerAccounts(accountPtrs);

    // OS Concept: Scheduler initialization
    Scheduler scheduler(SchedulingPolicy::FCFS);
    cout << "Scheduler initialized with policy: " 
         << scheduler.getPolicyName() << "\n" << endl;

    // Main menu loop
    bool running = true;
    while (running) {
        displayMenu();
        
        int choice;
        cin >> choice;

        // Handle invalid input
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a number.\n" << endl;
            continue;
        }

        switch (choice) {
            case 1: {  // Deposit
                int accId;
                double amount;
                cout << "Enter account ID (1-" << accounts.size() << "): ";
                cin >> accId;
                cout << "Enter amount to deposit: $";
                cin >> amount;

                if (accId >= 1 && accId <= (int)accounts.size() && amount > 0) {
                    performDeposit(*accounts[accId - 1], amount);
                } else {
                    cout << "Invalid account ID or amount." << endl;
                }
                break;
            }
            
            case 2: {  // Withdraw
                int accId;
                double amount;
                cout << "Enter account ID (1-" << accounts.size() << "): ";
                cin >> accId;
                cout << "Enter amount to withdraw: $";
                cin >> amount;

                if (accId >= 1 && accId <= (int)accounts.size() && amount > 0) {
                    performWithdrawal(*accounts[accId - 1], amount);
                } else {
                    cout << "Invalid account ID or amount." << endl;
                }
                break;
            }
            
            case 3: {  // Transfer
                int fromId, toId;
                double amount;
                cout << "Enter source account ID (1-" << accounts.size() << "): ";
                cin >> fromId;
                cout << "Enter destination account ID (1-" << accounts.size() << "): ";
                cin >> toId;
                cout << "Enter amount to transfer: $";
                cin >> amount;

                if (fromId >= 1 && fromId <= (int)accounts.size() &&
                    toId >= 1 && toId <= (int)accounts.size() &&
                    fromId != toId && amount > 0) {
                    performTransfer(*accounts[fromId - 1], *accounts[toId - 1], amount);
                } else {
                    cout << "Invalid account IDs or amount." << endl;
                }
                break;
            }
            
            case 4:  // Show Accounts
                displayAccounts(accounts);
                break;
            
            case 5:  // Run Demo
                runDemoTransactions(accounts, scheduler);
                displayAccounts(accounts);
                break;
            
            case 6: {  // Change Scheduling Policy
                cout << "\nSelect scheduling policy:" << endl;
                cout << "1. FCFS (First-Come-First-Served)" << endl;
                cout << "2. Priority-Based" << endl;
                cout << "Enter choice: ";
                
                int policyChoice;
                cin >> policyChoice;
                
                if (policyChoice == 1) {
                    scheduler.setPolicy(SchedulingPolicy::FCFS);
                } else if (policyChoice == 2) {
                    scheduler.setPolicy(SchedulingPolicy::PRIORITY);
                } else {
                    cout << "Invalid choice." << endl;
                }
                break;
            }
            
           // In main.cpp, inside the main() function's switch statement
// ...
case 7:
    deadlockHandler.detectDeadlock();
    break;
case 8: // <-- ADD THIS CASE
    DeadlockDemo::demonstrateDeadlock();
    break;
case 9: // <-- CHANGE EXIT TO 9
    cout << "\nShutting down banking system...\n";
    running = false;
    break;
// ...
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
        
        // Small delay for readability
        this_thread::sleep_for(chrono::milliseconds(200));
    }

    cout << "\n=== Banking System Terminated ===" << endl;
    cout << "Total deadlock detection runs: " 
         << deadlockHandler.getDetectionCount() << endl;
    cout << "Log file: transaction_log.txt" << endl;
    
    return 0;
}