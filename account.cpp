// account.cpp
// Implementation of thread-safe Account operations
#include "account.h"
#include <sstream>
#include <iomanip>

using namespace std;

Account::Account(int accountId, double initialBalance) 
    : id(accountId), balance(initialBalance) {}

int Account::getId() const {
    return id;
}

double Account::getBalance() const {
    return balance;
}

// OS Concept: Mutex lock_guard for automatic lock/unlock (RAII pattern)
bool Account::deposit(double amount) {
    if (amount <= 0) {
        return false;
    }

    lock_guard<mutex> guard(lock);  // Critical section begins
    balance += amount;
    return true;  // Lock automatically released when guard goes out of scope
}

bool Account::withdraw(double amount) {
    if (amount <= 0) {
        return false;
    }

    lock_guard<mutex> guard(lock);  // Critical section begins
    if (balance >= amount) {
        balance -= amount;
        return true;
    }
    return false;  // Insufficient funds
}

// OS Concept: Multiple mutex acquisition - potential deadlock scenario
bool Account::transfer(Account& toAccount, double amount) {
    if (amount <= 0 || this->id == toAccount.id) {
        return false;
    }

    // OS Concept: Lock ordering to prevent deadlock
    // Always lock accounts in ascending order of ID
    Account* first = (this->id < toAccount.id) ? this : &toAccount;
    Account* second = (this->id < toAccount.id) ? &toAccount : this;

    lock_guard<mutex> lock1(first->lock);
    lock_guard<mutex> lock2(second->lock);

    if (this->balance >= amount) {
        this->balance -= amount;
        toAccount.balance += amount;
        return true;
    }
    return false;  // Insufficient funds
}

string Account::toString() const {
    ostringstream oss;
    oss << "Account[ID=" << id << ", Balance=$" 
        << fixed << setprecision(2) << balance << "]";
    return oss.str();
}

mutex& Account::getMutex() {
    return lock;
}