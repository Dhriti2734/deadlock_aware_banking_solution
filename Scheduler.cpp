#include "Scheduler.h"
#include <iostream>
#include <algorithm>

Scheduler::Scheduler(SchedulingPolicy policy) 
    : policy(policy), stopScheduler(false) {}

void Scheduler::setPolicy(SchedulingPolicy newPolicy) {
    std::lock_guard<std::mutex> lock(queueMutex);
    policy = newPolicy;
    std::cout << "> Scheduler: Policy changed to " << getPolicyName() << std::endl;
}

SchedulingPolicy Scheduler::getPolicy() const {
    return policy;
}

std::string Scheduler::getPolicyName() const {
    return (policy == SchedulingPolicy::FCFS) ? "FCFS" : "Priority";
}

void Scheduler::addTransaction(Transaction* transaction) {
    std::lock_guard<std::mutex> lock(queueMutex);
    
    if (policy == SchedulingPolicy::FCFS) {
        fcfsQueue.push(transaction);
        std::cout << "> Scheduler: Adding transaction " << transaction->getId() 
                  << " to FCFS queue." << std::endl;
    } else {
        priorityQueue.push_back(transaction);
        std::sort(priorityQueue.begin(), priorityQueue.end(), PriorityComparator());
        
        std::string prioStr = (transaction->getPriority() == Priority::HIGH) ? "High" :
                             (transaction->getPriority() == Priority::MEDIUM) ? "Medium" : "Low";
        std::cout << "> Scheduler (Priority): Queued " << transaction->getId() 
                  << " [Priority: " << prioStr << "]" << std::endl;
    }
    
    cv.notify_one();
}

void Scheduler::applyAging() {
    // Check waiting times and increase priority for starving transactions
    auto now = std::chrono::steady_clock::now();
    
    for (auto* trans : priorityQueue) {
        auto waitTime = std::chrono::duration_cast<std::chrono::seconds>(
            now - trans->getCreationTime()).count();
        
        if (waitTime > 5 && trans->getPriority() != Priority::HIGH) {
            trans->increasePriority();
            std::cout << "> Scheduler (Aging): " << trans->getId() 
                      << " waiting too long. Increasing priority." << std::endl;
        }
    }
    
    // Re-sort after aging
    std::sort(priorityQueue.begin(), priorityQueue.end(), PriorityComparator());
}

Transaction* Scheduler::getNextTransaction() {
    std::unique_lock<std::mutex> lock(queueMutex);
    
    if (policy == SchedulingPolicy::FCFS) {
        if (fcfsQueue.empty()) return nullptr;
        Transaction* trans = fcfsQueue.front();
        fcfsQueue.pop();
        return trans;
    } else {
        applyAging();
        
        if (priorityQueue.empty()) return nullptr;
        Transaction* trans = priorityQueue.front();
        priorityQueue.erase(priorityQueue.begin());
        
        std::string prioStr = (trans->getPriority() == Priority::HIGH) ? "High" :
                             (trans->getPriority() == Priority::MEDIUM) ? "Medium" : "Low";
        std::cout << "> Scheduler (Priority): Executing " << trans->getId() 
                  << " (" << prioStr << " Priority)." << std::endl;
        
        return trans;
    }
}

bool Scheduler::isEmpty() {
    std::lock_guard<std::mutex> lock(queueMutex);
    return (policy == SchedulingPolicy::FCFS) ? fcfsQueue.empty() : priorityQueue.empty();
}

int Scheduler::getQueueSize() {
    std::lock_guard<std::mutex> lock(queueMutex);
    return (policy == SchedulingPolicy::FCFS) ? fcfsQueue.size() : priorityQueue.size();
}

void Scheduler::stop() {
    stopScheduler = true;
    cv.notify_all();
}