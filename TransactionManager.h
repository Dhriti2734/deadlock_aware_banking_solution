#ifndef TRANSACTION_MANAGER_H
#define TRANSACTION_MANAGER_H

#include "Account.h"
#include "Transaction.h"
#include "Scheduler.h"
#include "DeadlockHandler.h"
#include "Logger.h"
#include <map>
#include <vector>
#include <thread>
#include <atomic>

class TransactionManager {
private:
    std::map<int, Account*>& accounts;
    Scheduler& scheduler;
    DeadlockHandler& deadlockHandler;
    Logger* logger;
    
    std::map<int, bool> lockedAccounts;
    std::mutex lockMapMutex;
    
    std::vector<std::thread> workerThreads;
    std::atomic<bool> running;
    
    std::vector<Transaction*> failedTransactions;
    std::mutex failedTransMutex;
    
    bool useBankersAlgorithm;
    
    void processTransaction(Transaction* transaction);
    void workerThread();
    
    bool executeDeposit(Transaction* transaction);
    bool executeWithdraw(Transaction* transaction);
    bool executeTransfer(Transaction* transaction);
    
    void acquireLocks(const std::vector<int>& accountIds);
    void releaseLocks(const std::vector<int>& accountIds);

public:
    TransactionManager(std::map<int, Account*>& accounts, 
                      Scheduler& scheduler,
                      DeadlockHandler& deadlockHandler);
    ~TransactionManager();
    
    void start(int numWorkers = 2);
    void stop();
    
    void submitTransaction(Transaction* transaction);
    void retryFailedTransactions();
    
    std::vector<Transaction*> getFailedTransactions();
    
    void setBankersAlgorithm(bool enabled);
    bool isBankersAlgorithmEnabled() const;
};

#endif // TRANSACTION_MANAGER_H