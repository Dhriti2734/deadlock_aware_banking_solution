#include "deadlock_demo.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std; // Added this line

// Define two global mutexes for the demonstration
mutex mutex1;
mutex mutex2;

void DeadlockDemo::demonstrateDeadlock() {
    cout << "\n=== INTENTIONAL DEADLOCK DEMO ===\n";
    cout << "WARNING: This will freeze the application.\n";
    cout << "You will need to close the terminal or press Ctrl+C to exit.\n";
    cout << "Launching two threads that will lock mutexes in opposite orders...\n\n";
    this_thread::sleep_for(chrono::seconds(2)); // Give user time to read

    // Thread 1: Locks mutex1, then tries to lock mutex2
    thread t1([]() {
        cout << "[Thread 1] Trying to acquire lock 1...\n";
        lock_guard<mutex> guard1(mutex1);
        cout << "[Thread 1] Acquired lock 1!\n";
        
        // Sleep to give Thread 2 a chance to acquire lock 2
        this_thread::sleep_for(chrono::milliseconds(100));
        
        cout << "[Thread 1] Trying to acquire lock 2...\n";
        lock_guard<mutex> guard2(mutex2); // <-- THIS WILL HANG
        cout << "[Thread 1] Acquired lock 2! (This message will not appear)\n";
    });

    // Thread 2: Locks mutex2, then tries to lock mutex1 (Opposite Order -> Deadlock!)
    thread t2([]() {
        cout << "[Thread 2] Trying to acquire lock 2...\n";
        lock_guard<mutex> guard2(mutex2);
        cout << "[Thread 2] Acquired lock 2!\n";

        // Sleep to give Thread 1 a chance to try for lock 2
        this_thread::sleep_for(chrono::milliseconds(100));

        cout << "[Thread 2] Trying to acquire lock 1...\n";
        lock_guard<mutex> guard1(mutex1); // <-- THIS WILL HANG
        cout << "[Thread 2] Acquired lock 1! (This message will not appear)\n";
    });

    // The main thread will also hang here waiting for t1 and t2, which will never finish.
    t1.join();
    t2.join();
}