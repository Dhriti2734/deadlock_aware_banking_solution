// scheduler.cpp
// Thread scheduling implementation
#include "scheduler.h"
#include <algorithm>
#include <iostream>

using namespace std;

Scheduler::Scheduler(SchedulingPolicy schedulingPolicy) : policy(schedulingPolicy) {}

void Scheduler::addTask(int threadId, int priority, const string& description,
                       function<void()> task) {
    taskQueue.emplace_back(threadId, priority, description, task);
}

// OS Concept: Scheduling algorithm implementation
void Scheduler::sortTasks() {
    switch (policy) {
        case SchedulingPolicy::FCFS:
            // FCFS: Tasks remain in the order they were added (no sorting needed)
            break;
        
        case SchedulingPolicy::PRIORITY:
            // OS Concept: Priority scheduling - sort by priority (ascending)
            sort(taskQueue.begin(), taskQueue.end(),
                 [](const ThreadTask& a, const ThreadTask& b) {
                     return a.priority < b.priority;  // Lower number = higher priority
                 });
            break;
    }
}

// OS Concept: Task execution and thread management
void Scheduler::executeTasks() {
    if (taskQueue.empty()) {
        cout << "[Scheduler] No tasks to execute." << endl;
        return;
    }

    sortTasks();

    cout << "\n[Scheduler: " << getPolicyName() << "] Executing " 
         << taskQueue.size() << " tasks..." << endl;

    vector<thread> threads;

    // OS Concept: Thread creation and management
    for (auto& taskDesc : taskQueue) {
        cout << "[Scheduler] Starting Thread-" << taskDesc.threadId 
             << " (Priority=" << taskDesc.priority << "): " 
             << taskDesc.description << endl;
        
        // Create and start thread
        threads.emplace_back(taskDesc.task);
        
        // Small delay for demonstration purposes (simulates scheduling)
        this_thread::sleep_for(chrono::milliseconds(50));
    }

    // OS Concept: Thread synchronization - wait for all threads to complete
    cout << "[Scheduler] Waiting for all threads to complete..." << endl;
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();  // OS Concept: Thread join operation
        }
    }

    cout << "[Scheduler] All tasks completed.\n" << endl;
    clearTasks();
}

void Scheduler::clearTasks() {
    taskQueue.clear();
}

string Scheduler::getPolicyName() const {
    switch (policy) {
        case SchedulingPolicy::FCFS:
            return "FCFS (First-Come-First-Served)";
        case SchedulingPolicy::PRIORITY:
            return "Priority-Based";
        default:
            return "Unknown";
    }
}

void Scheduler::setPolicy(SchedulingPolicy newPolicy) {
    policy = newPolicy;
    cout << "[Scheduler] Policy changed to: " << getPolicyName() << endl;
}