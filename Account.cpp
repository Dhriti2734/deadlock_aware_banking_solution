#include "Account.h"
#include <sstream>
#include <iomanip>

Account::Account(int id, double initialBalance) 
    : accountId(id), balance(initialBalance) {}

int Account::getId() const {
    return accountId;
}

double Account::getBalance() const {
    return balance;
}

bool Account::deposit(double amount) {
    if (amount <= 0) return false;
    balance += amount;
    return true;
}

bool Account::withdraw(double amount) {
    if (amount <= 0 || balance < amount) return false;
    balance -= amount;
    return true;
}

std::mutex& Account::getMutex() {
    return accountMutex;
}

std::string Account::toString() const {
    std::ostringstream oss;
    oss << "Account " << accountId << " (Balance: $" 
        << std::fixed << std::setprecision(2) << balance << ")";
    return oss.str();
}