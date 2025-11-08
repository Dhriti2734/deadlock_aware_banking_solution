/**
 * TransactionManager.cpp
 * Implementation of multi-threaded transaction processing system
 * Manages worker threads, transaction execution, and deadlock prevention
 */

#include "TransactionManager.h"
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>

/**
 * Constructor: Initialize transaction manager
 * Sets up references to accounts, scheduler, and deadlock handler
 */
TransactionManager::TransactionManager(std::map<int, Account*>& accounts,
                                       Scheduler& scheduler,
                                       DeadlockHandler& deadlockHandler)
    : accounts(accounts), scheduler(scheduler), deadlockHandler(deadlockHandler),
      running(false), useBankersAlgorithm(true) {
    
    logger = Logger::getInstance();
    
    // Initialize lock tracking map for all accounts
    for (const auto& pair : accounts) {
        lockedAccounts[pair.first] = false;
    }
}

/**
 * Destructor: Ensure clean shutdown
 */
TransactionManager::~TransactionManager() {
    stop();
}

/**
 * Start the transaction manager with specified number of worker threads
 * Each worker thread continuously processes transactions from the scheduler
 */
void TransactionManager::start(int numWorkers) {
    running = true;
    
    // Create worker threads
    for (int i = 0; i < numWorkers; ++i) {
        workerThreads.emplace_back(&TransactionManager::workerThread, this);
    }
}

/**
 * Stop all worker threads gracefully
 * Waits for all threads to finish current work
 */
void TransactionManager::stop() {
    running = false;
    scheduler.stop();
    
    // Join all worker threads
    for (auto& thread : workerThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    workerThreads.clear();
}

/**
 * Submit a new transaction to the scheduler
 */
void TransactionManager::submitTransaction(Transaction* transaction) {
    std::cout << "\n[Transaction " << transaction->getId() << " Queued: " 
              << transaction->toString() << "]" << std::endl;
    scheduler.addTransaction(transaction);
}

/**
 * Enable or disable Banker's Algorithm
 * When disabled, naive locking is used (can cause deadlock)
 */
void TransactionManager::setBankersAlgorithm(bool enabled) {
    useBankersAlgorithm = enabled;
}

/**
 * Check if Banker's Algorithm is currently enabled
 */
bool TransactionManager::isBankersAlgorithmEnabled() const {
    return useBankersAlgorithm;
}

/**
 * Worker thread function
 * Continuously fetches and processes transactions from scheduler
 */
void TransactionManager::workerThread() {
    while (running) {
        // Get next transaction from scheduler
        Transaction* transaction = scheduler.getNextTransaction();
        
        if (transaction == nullptr) {
            // No transactions available, sleep briefly
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        
        // Process the transaction
        processTransaction(transaction);
    }
}

/**
 * Process a single transaction
 * Performs safety check (if enabled), acquires locks, executes transaction
 */
void TransactionManager::processTransaction(Transaction* transaction) {
    std::cout << "\n> Scheduler: Executing " << transaction->getId() << std::endl;
    
    transaction->setStatus(TransactionStatus::PROCESSING);
    
    // CRITICAL: Check safe state with Banker's Algorithm if enabled
    if (useBankersAlgorithm) {
        std::lock_guard<std::mutex> lock(lockMapMutex);
        
        // Run Banker's Algorithm to check if proceeding is safe
        bool safe = deadlockHandler.checkSafeState(transaction, lockedAccounts);
        
        if (!safe) {
            // UNSAFE state detected - transaction must wait
            std::cout << "> Transaction " << transaction->getId() 
                      << ": UNSAFE state detected. Marking as WAITING." << std::endl;
            
            transaction->setStatus(TransactionStatus::UNSAFE);
            transaction->incrementWaitCount();
            
            // Log the unsafe transaction
            logger->logTransaction(transaction->toLogString());
            
            // Add to failed transactions for retry
            std::lock_guard<std::mutex> failLock(failedTransMutex);
            failedTransactions.push_back(transaction);
            return;
        }
    }
    
    // Safe to proceed - execute based on transaction type
    bool success = false;
    
    switch (transaction->getType()) {
        case TransactionType::DEPOSIT:
            success = executeDeposit(transaction);
            break;
        case TransactionType::WITHDRAW:
            success = executeWithdraw(transaction);
            break;
        case TransactionType::TRANSFER:
            success = executeTransfer(transaction);
            break;
    }
    
    // Update transaction status
    if (success) {
        transaction->setStatus(TransactionStatus::SUCCESS);
        std::cout << "> Transaction: Success." << std::endl;
    } else {
        transaction->setStatus(TransactionStatus::FAILED);
        std::cout << "> Transaction: Failed." << std::endl;
    }
    
    // Log the transaction result
    logger->logTransaction(transaction->toLogString());
}

/**
 * Acquire locks for specified accounts in order
 * Updates locked accounts map for deadlock detection
 */
void TransactionManager::acquireLocks(const std::vector<int>& accountIds) {
    for (int id : accountIds) {
        std::cout << "> Sync: Acquiring lock for Account " << id << "..." << std::endl;
        accounts[id]->getMutex().lock();
        
        // Update lock tracking
        std::lock_guard<std::mutex> lock(lockMapMutex);
        lockedAccounts[id] = true;
    }
}

/**
 * Release locks for specified accounts
 * Updates locked accounts map
 */
void TransactionManager::releaseLocks(const std::vector<int>& accountIds) {
    std::cout << "> Sync: Releasing locks." << std::endl;
    
    for (int id : accountIds) {
        accounts[id]->getMutex().unlock();
        
        // Update lock tracking
        std::lock_guard<std::mutex> lock(lockMapMutex);
        lockedAccounts[id] = false;
    }
}

/**
 * Execute a deposit transaction
 * Locks source account, performs deposit, releases lock
 */
bool TransactionManager::executeDeposit(Transaction* transaction) {
    std::vector<int> locks = {transaction->getSourceAccountId()};
    acquireLocks(locks);
    
    std::cout << "> Transaction: Processing..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Simulate processing
    
    bool success = accounts[transaction->getSourceAccountId()]->deposit(transaction->getAmount());
    
    releaseLocks(locks);
    return success;
}

/**
 * Execute a withdraw transaction
 * Locks source account, performs withdrawal, releases lock
 */
bool TransactionManager::executeWithdraw(Transaction* transaction) {
    std::vector<int> locks = {transaction->getSourceAccountId()};
    acquireLocks(locks);
    
    std::cout << "> Transaction: Processing..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Simulate processing
    
    bool success = accounts[transaction->getSourceAccountId()]->withdraw(transaction->getAmount());
    
    releaseLocks(locks);
    return success;
}

/**
 * Execute a transfer transaction
 * Locks both accounts IN ORDER (prevents circular wait), performs transfer
 */
bool TransactionManager::executeTransfer(Transaction* transaction) {
    // Get accounts in sorted order (critical for deadlock prevention)
    std::vector<int> locks = deadlockHandler.getRequiredAccounts(transaction);
    acquireLocks(locks);
    
    std::cout << "> Transaction: Processing..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Simulate processing
    
    int srcId = transaction->getSourceAccountId();
    int destId = transaction->getDestAccountId();
    double amount = transaction->getAmount();
    
    // Atomic transfer operation
    bool success = false;
    if (accounts[srcId]->withdraw(amount)) {
        accounts[destId]->deposit(amount);
        success = true;
    }
    
    releaseLocks(locks);
    return success;
}

/**
 * Retry all failed/unsafe transactions
 * Resubmits them to scheduler for another attempt
 */
void TransactionManager::retryFailedTransactions() {
    std::lock_guard<std::mutex> lock(failedTransMutex);
    
    if (failedTransactions.empty()) {
        std::cout << "\n> No failed transactions to retry." << std::endl;
        return;
    }
    
    std::cout << "\n> Retrying " << failedTransactions.size() << " failed transaction(s)..." << std::endl;
    
    // Resubmit each failed transaction
    for (auto* trans : failedTransactions) {
        trans->setStatus(TransactionStatus::PENDING);
        scheduler.addTransaction(trans);
    }
    
    failedTransactions.clear();
}

/**
 * Get list of all failed transactions
 */
std::vector<Transaction*> TransactionManager::getFailedTransactions() {
    std::lock_guard<std::mutex> lock(failedTransMutex);
    return failedTransactions;
}