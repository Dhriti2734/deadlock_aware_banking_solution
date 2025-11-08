#ifndef DEADLOCK_HANDLER_H
#define DEADLOCK_HANDLER_H

#include "Account.h"
#include "Transaction.h"
#include <vector>
#include <map>
#include <mutex>

class DeadlockHandler {
private:
    std::map<int, Account*>& accounts;
    std::mutex safetyMutex;
    
    bool isSafeState(const std::vector<int>& requestedAccounts, 
                     const std::map<int, bool>& currentlyLocked);

public:
    DeadlockHandler(std::map<int, Account*>& accountMap);
    
    bool checkSafeState(Transaction* transaction, 
                       const std::map<int, bool>& currentlyLocked);
    
    std::vector<int> getRequiredAccounts(Transaction* transaction);
};

#endif // DEADLOCK_HANDLER_H