/**
 * main.cpp
 * Entry point for the Deadlock Aware Banking System
 * 
 * This program demonstrates:
 * - Multi-threaded transaction processing
 * - FCFS and Priority CPU scheduling with aging
 * - Banker's Algorithm for deadlock prevention
 * - Mutex-based synchronization
 * - File-based logging
 */

#include "BankingSystem.h"
#include <iostream>

int main() {
    try {
        // Create and run the banking system
        BankingSystem system;
        system.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}