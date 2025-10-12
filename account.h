// account.h
// OS Concepts: Thread Synchronization, Mutual Exclusion, Critical Sections
#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <mutex>
#include <string>

using namespace std;

class Account {
private:
    int id;
    double balance;
    mutex lock;  // OS Concept: Mutex for thread-safe operations

public:
    // Constructor
    Account(int accountId, double initialBalance);

    // Getters
    int getId() const;
    double getBalance() const;

    // Thread-safe transaction methods
    // OS Concept: Critical Section Protection
    bool deposit(double amount);
    bool withdraw(double amount);
    bool transfer(Account& toAccount, double amount);

    // Display account information
    string toString() const;

    // Allow mutex access for advanced locking strategies
    mutex& getMutex();
};

#endif // ACCOUNT_H