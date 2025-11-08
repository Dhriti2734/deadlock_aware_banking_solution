/**
 * Account.h
 * Header file for the Account class
 * Manages individual bank account data with thread-safe operations
 */

#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <mutex>
#include <string>

class Account {
private:
    int accountId;                      // Unique account identifier
    double balance;                     // Current account balance
    mutable std::mutex accountMutex;    // Mutex for thread-safe operations

public:
    /**
     * Constructor: Initialize account with ID and balance
     * @param id Account ID
     * @param initialBalance Starting balance
     */
    Account(int id, double initialBalance);
    
    // Getters
    int getId() const;
    double getBalance() const;
    
    /**
     * Deposit money into account
     * @param amount Amount to deposit
     * @return true if successful, false otherwise
     */
    bool deposit(double amount);
    
    /**
     * Withdraw money from account
     * @param amount Amount to withdraw
     * @return true if successful, false if insufficient funds
     */
    bool withdraw(double amount);
    
    /**
     * Get reference to account's mutex for external locking
     * @return Reference to the account mutex
     */
    std::mutex& getMutex();
    
    /**
     * Convert account info to string representation
     * @return Formatted account string
     */
    std::string toString() const;
};

#endif // ACCOUNT_H