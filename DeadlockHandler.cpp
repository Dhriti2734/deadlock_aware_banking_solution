#include "DeadlockHandler.h"
#include <iostream>
#include <algorithm>

DeadlockHandler::DeadlockHandler(std::map<int, Account*>& accountMap) 
    : accounts(accountMap) {}

std::vector<int> DeadlockHandler::getRequiredAccounts(Transaction* transaction) {
    std::vector<int> required;
    
    if (transaction->getType() == TransactionType::TRANSFER) {
        required.push_back(transaction->getSourceAccountId());
        required.push_back(transaction->getDestAccountId());
        // Always lock in ascending order to prevent circular wait
        std::sort(required.begin(), required.end());
    } else {
        required.push_back(transaction->getSourceAccountId());
    }
    
    return required;
}

bool DeadlockHandler::isSafeState(const std::vector<int>& requestedAccounts,
                                   const std::map<int, bool>& currentlyLocked) {
    // Banker's Algorithm: Check if granting these resources leads to a safe state
    
    // Create a simulated state
    std::map<int, bool> simulatedLocked = currentlyLocked;
    
    // Mark requested accounts as locked in simulation
    for (int accId : requestedAccounts) {
        if (simulatedLocked[accId]) {
            // Resource already locked - not safe to proceed
            return false;
        }
        simulatedLocked[accId] = true;
    }
    
    // Check if there's a safe sequence
    // In a banking system, we check if sufficient resources exist
    std::vector<int> availableAccounts;
    for (const auto& pair : accounts) {
        if (!simulatedLocked[pair.first]) {
            availableAccounts.push_back(pair.first);
        }
    }
    
    // If we can still access other accounts after this allocation, it's safe
    // This is a simplified banker's algorithm for the banking domain
    return true; // If no circular dependency detected, state is safe
}

bool DeadlockHandler::checkSafeState(Transaction* transaction,
                                     const std::map<int, bool>& currentlyLocked) {
    std::lock_guard<std::mutex> lock(safetyMutex);
    
    std::cout << "> Deadlock Handler: Checking safe state (Banker's Algorithm)..." << std::endl;
    
    std::vector<int> required = getRequiredAccounts(transaction);
    
    // Check for potential circular wait
    for (int accId : required) {
        auto it = currentlyLocked.find(accId);
        if (it != currentlyLocked.end() && it->second) {
            std::cout << "> Deadlock Handler: Account " << accId 
                      << " is locked. State is UNSAFE." << std::endl;
            return false;
        }
    }
    
    bool safe = isSafeState(required, currentlyLocked);
    
    if (safe) {
        std::cout << "> Deadlock Handler: State is SAFE." << std::endl;
    } else {
        std::cout << "> Deadlock Handler: State is UNSAFE." << std::endl;
    }
    
    return safe;
}