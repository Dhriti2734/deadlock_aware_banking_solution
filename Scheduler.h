#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Transaction.h"
#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <functional>

enum class SchedulingPolicy {
    FCFS,
    PRIORITY
};

class Scheduler {
private:
    SchedulingPolicy policy;
    std::queue<Transaction*> fcfsQueue;
    std::vector<Transaction*> priorityQueue;
    std::mutex queueMutex;
    std::condition_variable cv;
    bool stopScheduler;
    
    struct PriorityComparator {
        bool operator()(Transaction* a, Transaction* b) {
            if (a->getPriority() != b->getPriority()) {
                return static_cast<int>(a->getPriority()) > static_cast<int>(b->getPriority());
            }
            return a->getCreationTime() < b->getCreationTime();
        }
    };
    
    void applyAging();

public:
    Scheduler(SchedulingPolicy policy = SchedulingPolicy::FCFS);
    
    void setPolicy(SchedulingPolicy newPolicy);
    SchedulingPolicy getPolicy() const;
    std::string getPolicyName() const;
    
    void addTransaction(Transaction* transaction);
    Transaction* getNextTransaction();
    
    bool isEmpty();
    int getQueueSize();
    
    void stop();
};

#endif // SCHEDULER_H