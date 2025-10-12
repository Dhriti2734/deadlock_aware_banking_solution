// scheduler.h
// OS Concepts: Thread Scheduling, Process Management
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include <string>
#include <functional>
#include <thread>

using namespace std;

// OS Concept: Thread descriptor structure
struct ThreadTask {
    int threadId;
    int priority;  // Lower value = higher priority (0 is highest)
    string description;
    function<void()> task;

    ThreadTask(int id, int prio, const string& desc, function<void()> t)
        : threadId(id), priority(prio), description(desc), task(t) {}
};

// OS Concept: Scheduling policies
enum class SchedulingPolicy {
    FCFS,      // First-Come-First-Served
    PRIORITY   // Priority-based scheduling
};

class Scheduler {
private:
    vector<ThreadTask> taskQueue;
    SchedulingPolicy policy;

    // Sort tasks based on scheduling policy
    void sortTasks();

public:
    // Constructor
    Scheduler(SchedulingPolicy schedulingPolicy = SchedulingPolicy::FCFS);

    // Add task to scheduler queue
    void addTask(int threadId, int priority, const string& description,
                 function<void()> task);

    // OS Concept: Execute scheduled tasks
    void executeTasks();

    // Clear all pending tasks
    void clearTasks();

    // Get current policy
    string getPolicyName() const;
    
    // Set scheduling policy
    void setPolicy(SchedulingPolicy newPolicy);
};

#endif // SCHEDULER_H