#ifndef BANKING_SYSTEM_H
#define BANKING_SYSTEM_H

#include "Account.h"
#include "Transaction.h"
#include "Scheduler.h"
#include "DeadlockHandler.h"
#include "TransactionManager.h"
#include "Logger.h"
#include <map>
#include <vector>

class BankingSystem {
private:
    std::map<int, Account*> accounts;
    Scheduler* scheduler;
    DeadlockHandler* deadlockHandler;
    TransactionManager* transactionManager;
    Logger* logger;
    
    std::vector<Transaction*> allTransactions;
    
    void initializeAccounts();
    void displayHeader();
    void displayAccounts();
    void displayMainMenu();
    
    void handleDeposit();
    void handleWithdraw();
    void handleTransfer();
    void handleConcurrentTest();
    void handleDeadlockDemo();
    void handleChangePolicy();
    void handleRetryFailed();
    void handleViewLog();
    
    void runDeadlockDemoNaive();
    void runDeadlockDemoBankers();

public:
    BankingSystem();
    ~BankingSystem();
    
    void run();
};

#endif // BANKING_SYSTEM_H