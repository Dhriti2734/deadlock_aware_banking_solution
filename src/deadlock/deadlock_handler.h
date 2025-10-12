// deadlock_handler.h
// OS Concepts: Deadlock Detection, Deadlock Prevention
#ifndef DEADLOCK_HANDLER_H
#define DEADLOCK_HANDLER_H

#include <vector>
#include <string>

using namespace std;

// Forward declaration
class Account;

class DeadlockHandler {
private:
    vector<Account*> accounts;
    int detectionCount;

public:
    // Constructor
    DeadlockHandler();

    // Register accounts for monitoring
    void registerAccounts(const vector<Account*>& accs);

    // OS Concept: Deadlock detection (placeholder for now)
    // Will implement Banker's Algorithm and Wait-For Graph in second half
    bool detectDeadlock();

    // OS Concept: Deadlock handling strategies (placeholder)
    // Will implement resource preemption and rollback in second half
    void handleDeadlock();

    // Get detection statistics
    int getDetectionCount() const;
    
    // Display deadlock status
    void displayStatus() const;
};

#endif // DEADLOCK_HANDLER_H