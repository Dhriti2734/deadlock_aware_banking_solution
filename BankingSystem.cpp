#include "BankingSystem.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <cstdlib>

BankingSystem::BankingSystem() {
    initializeAccounts();
    
    scheduler = new Scheduler(SchedulingPolicy::FCFS);
    deadlockHandler = new DeadlockHandler(accounts);
    transactionManager = new TransactionManager(accounts, *scheduler, *deadlockHandler);
    logger = Logger::getInstance();
    
    transactionManager->start(3);
}

BankingSystem::~BankingSystem() {
    transactionManager->stop();
    
    delete transactionManager;
    delete deadlockHandler;
    delete scheduler;
    
    for (auto& pair : accounts) {
        delete pair.second;
    }
    
    for (auto* trans : allTransactions) {
        delete trans;
    }
}

void BankingSystem::initializeAccounts() {
    accounts[101] = new Account(101, 1000.0);
    accounts[102] = new Account(102, 1000.0);
    accounts[103] = new Account(103, 500.0);
}

void BankingSystem::displayHeader() {
    // system("cls"); // <--- PROBLEM SOLVED!

    // Optional: Add a newline to separate from previous output
    std::cout << "\n\n" << std::endl;
    std::cout << "=========================================" << std::endl;
    std::cout << "   Deadlock Aware Banking System" << std::endl;
    std::cout << "=========================================" << std::endl;
}

void BankingSystem::displayAccounts() {
    std::cout << "\n[Accounts]" << std::endl;
    for (const auto& pair : accounts) {
        std::cout << "- " << pair.second->toString() << std::endl;
    }
}

void BankingSystem::displayMainMenu() {
    std::cout << "\n[Current Scheduler: " << scheduler->getPolicyName() << "]" << std::endl;
    std::cout << "\n[Main Menu]" << std::endl;
    std::cout << "1. Deposit" << std::endl;
    std::cout << "2. Withdraw" << std::endl;
    std::cout << "3. Manual Transfer" << std::endl;
    std::cout << "4. Run Concurrent Test (Simulate 5 random transfers)" << std::endl;
    std::cout << "5. Run Deadlock Demonstration" << std::endl;
    std::cout << "6. Change Scheduling Policy" << std::endl;
    std::cout << "7. Retry Failed Transactions" << std::endl;
    std::cout << "8. View Transaction Log" << std::endl;
    std::cout << "9. Exit" << std::endl;
    std::cout << "\nEnter your choice: ";
}

void BankingSystem::run() {
    int choice;
    
    while (true) {
        displayHeader();
        displayAccounts();
        displayMainMenu();
        
        std::cin >> choice;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }
        
        switch (choice) {
            case 1: handleDeposit(); break;
            case 2: handleWithdraw(); break;
            case 3: handleTransfer(); break;
            case 4: handleConcurrentTest(); break;
            case 5: handleDeadlockDemo(); break;
            case 6: handleChangePolicy(); break;
            case 7: handleRetryFailed(); break;
            case 8: handleViewLog(); break;
            case 9:
                std::cout << "\nExiting system..." << std::endl;
                return;
            default:
                std::cout << "\nInvalid choice. Press Enter to continue...";
                std::cin.ignore();
                std::cin.get();
        }
    }
}

void BankingSystem::handleDeposit() {
    int accountId;
    double amount;
    
    std::cout << "\nEnter Account ID: ";
    std::cin >> accountId;
    std::cout << "Enter Amount: $";
    std::cin >> amount;
    
    if (accounts.find(accountId) == accounts.end()) {
        std::cout << "Account not found!" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        return;
    }
    
    Transaction* trans = new Transaction(TransactionType::DEPOSIT, accountId, 0, amount);
    allTransactions.push_back(trans);
    transactionManager->submitTransaction(trans);
    
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

void BankingSystem::handleWithdraw() {
    int accountId;
    double amount;
    
    std::cout << "\nEnter Account ID: ";
    std::cin >> accountId;
    std::cout << "Enter Amount: $";
    std::cin >> amount;
    
    if (accounts.find(accountId) == accounts.end()) {
        std::cout << "Account not found!" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        return;
    }
    
    Transaction* trans = new Transaction(TransactionType::WITHDRAW, accountId, 0, amount);
    allTransactions.push_back(trans);
    transactionManager->submitTransaction(trans);
    
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

void BankingSystem::handleTransfer() {
    int srcId, destId;
    double amount;
    
    std::cout << "\nEnter Source Account ID: ";
    std::cin >> srcId;
    std::cout << "Enter Destination Account ID: ";
    std::cin >> destId;
    std::cout << "Enter Amount: $";
    std::cin >> amount;
    
    if (accounts.find(srcId) == accounts.end() || accounts.find(destId) == accounts.end()) {
        std::cout << "Account not found!" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        return;
    }
    
    Transaction* trans = new Transaction(TransactionType::TRANSFER, srcId, destId, amount);
    allTransactions.push_back(trans);
    transactionManager->submitTransaction(trans);
    
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

void BankingSystem::handleConcurrentTest() {
    std::cout << "\n> Running Concurrent Test (using " << scheduler->getPolicyName() 
              << " Scheduler)..." << std::endl;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> accountDist(0, 2);
    std::uniform_int_distribution<> amountDist(50, 300);
    std::uniform_int_distribution<> priorityDist(0, 2);
    
    std::vector<int> accountIds = {101, 102, 103};
    
    for (int i = 0; i < 5; ++i) {
        int srcIdx = accountDist(gen);
        int destIdx = accountDist(gen);
        
        while (srcIdx == destIdx) {
            destIdx = accountDist(gen);
        }
        
        int srcId = accountIds[srcIdx];
        int destId = accountIds[destIdx];
        double amount = amountDist(gen);
        Priority prio = static_cast<Priority>(priorityDist(gen));
        
        Transaction* trans = new Transaction(TransactionType::TRANSFER, srcId, destId, amount, prio);
        allTransactions.push_back(trans);
        transactionManager->submitTransaction(trans);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    
    std::cout << "\n> Test queued. Transactions are being processed..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(4));
    std::cout << "\n> Test complete. Press Enter to continue...";
    std::cin.ignore();
    std::cin.get();
}

void BankingSystem::handleDeadlockDemo() {
    int choice;
    
    while (true) {
        displayHeader();
        std::cout << "\n[Deadlock Demonstration Menu]" << std::endl;
        std::cout << "This demo will run transfers concurrently, designed to deadlock:" << std::endl;
        std::cout << "- T1: Transfer $500 from Account 101 to 102" << std::endl;
        std::cout << "- T2: Transfer $500 from Account 102 to 101" << std::endl;
        std::cout << "\nPlease select a mode:" << std::endl;
        std::cout << "1. Run Demo (Mode 1: Naive Locking) -> *EXPECTED TO FREEZE*" << std::endl;
        std::cout << "2. Run Demo (Mode 2: Banker's Algorithm) -> *EXPECTED TO SUCCEED*" << std::endl;
        std::cout << "3. Back to Main Menu" << std::endl;
        std::cout << "\nEnter your choice: ";
        
        std::cin >> choice;
        
        if (choice == 1) {
            runDeadlockDemoNaive();
            break;
        } else if (choice == 2) {
            runDeadlockDemoBankers();
            break;
        } else if (choice == 3) {
            break;
        }
    }
}

void BankingSystem::runDeadlockDemoNaive() {
    std::cout << "\n> Starting Demo Mode 1 (Naive Locking)..." << std::endl;
    std::cout << "> WARNING: Banker's Algorithm disabled. System may deadlock!" << std::endl;
    
    transactionManager->setBankersAlgorithm(false);
    
    std::thread t1([this]() {
        std::cout << "[T1]: Thread started. Attempting to lock Account 101... " << std::endl;
        accounts[101]->getMutex().lock();
        std::cout << "[T1]: ACQUIRED Account 101." << std::endl;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "\n(Simulating context switch...)\n" << std::endl;
        
        std::cout << "[T1]: Attempting to lock Account 102... WAITING (Locked by T2)." << std::endl;
        accounts[102]->getMutex().lock();
        
        accounts[101]->withdraw(500);
        accounts[102]->deposit(500);
        
        accounts[102]->getMutex().unlock();
        accounts[101]->getMutex().unlock();
    });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    std::thread t2([this]() {
        std::cout << "[T2]: Thread started. Attempting to lock Account 102... " << std::endl;
        accounts[102]->getMutex().lock();
        std::cout << "[T2]: ACQUIRED Account 102." << std::endl;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        
        std::cout << "[T2]: Attempting to lock Account 101... WAITING (Locked by T1)." << std::endl;
        accounts[101]->getMutex().lock();
        
        accounts[102]->withdraw(500);
        accounts[101]->deposit(500);
        
        accounts[101]->getMutex().unlock();
        accounts[102]->getMutex().unlock();
    });
    
    std::cout << "\n*** SYSTEM DEADLOCK ***" << std::endl;
    std::cout << "(Circular Wait: T1 is waiting for T2, and T2 is waiting for T1)" << std::endl;
    std::cout << "(The console will now hang. Press Ctrl+C to terminate)" << std::endl;
    
    t1.join();
    t2.join();
}

void BankingSystem::runDeadlockDemoBankers() {
    std::cout << "\n> Starting Demo Mode 2 (Banker's Algorithm Enabled)..." << std::endl;
    
    transactionManager->setBankersAlgorithm(true);
    
    Transaction* t1 = new Transaction(TransactionType::TRANSFER, 101, 102, 500.0);
    Transaction* t2 = new Transaction(TransactionType::TRANSFER, 102, 101, 500.0);
    
    allTransactions.push_back(t1);
    allTransactions.push_back(t2);
    
    std::cout << "\n[Scheduler]: Queued T1 (101 -> 102, $500)" << std::endl;
    transactionManager->submitTransaction(t1);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    std::cout << "[Scheduler]: Queued T2 (102 -> 101, $500)" << std::endl;
    transactionManager->submitTransaction(t2);
    
    std::this_thread::sleep_for(std::chrono::seconds(3));
    
    std::cout << "\n> Demo Complete. Deadlock was successfully prevented." << std::endl;
    displayAccounts();
    
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
    std::cin.get();
}

void BankingSystem::handleChangePolicy() {
    int choice;
    
    std::cout << "\nSelect new scheduling policy:" << std::endl;
    std::cout << "1. FCFS (First-Come-First-Served)" << std::endl;
    std::cout << "2. Priority (with Aging)" << std::endl;
    std::cout << "\nYour choice: ";
    std::cin >> choice;
    
    if (choice == 1) {
        scheduler->setPolicy(SchedulingPolicy::FCFS);
        std::cout << "> System policy updated to: FCFS Scheduling" << std::endl;
    } else if (choice == 2) {
        scheduler->setPolicy(SchedulingPolicy::PRIORITY);
        std::cout << "> System policy updated to: Priority Scheduling" << std::endl;
    } else {
        std::cout << "Invalid choice!" << std::endl;
    }
    
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

void BankingSystem::handleRetryFailed() {
    transactionManager->retryFailedTransactions();
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

void BankingSystem::handleViewLog() {
    std::cout << "\n========== Transaction Log ==========" << std::endl;
    std::string logContent = logger->readLog();
    
    if (logContent.empty()) {
        std::cout << "(Log is empty)" << std::endl;
    } else {
        std::cout << logContent << std::endl;
    }
    
    std::cout << "=====================================" << std::endl;
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
    std::cin.get();
}