// logger.cpp
// Thread-safe logging implementation
#include "logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

using namespace std;

Logger::Logger(const string& logFilename) : filename(logFilename) {
    logFile.open(filename, ios::out | ios::app);
    if (!logFile.is_open()) {
        cerr << "ERROR: Could not open log file: " << filename << endl;
    } else {
        logMessage("=== Banking System Started ===");
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logMessage("=== Banking System Shutdown ===");
        logFile.close();
    }
}

// OS Concept: System time retrieval
string Logger::getCurrentTimestamp() const {
    auto now = chrono::system_clock::now();
    auto time = chrono::system_clock::to_time_t(now);
    auto ms = chrono::duration_cast<chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    ostringstream oss;
    oss << put_time(localtime(&time), "%Y-%m-%d %H:%M:%S");
    oss << "." << setfill('0') << setw(3) << ms.count();
    return oss.str();
}

// OS Concept: Critical section for file writing
void Logger::logTransaction(int threadId, const string& operation,
                           int accountId, double amount, const string& status) {
    lock_guard<mutex> guard(logMutex);
    if (logFile.is_open()) {
        logFile << "[" << getCurrentTimestamp() << "] "
                << "Thread-" << threadId << " | "
                << operation << " | "
                << "Account-" << accountId << " | "
                << "Amount: $" << fixed << setprecision(2) << amount << " | "
                << "Status: " << status << endl;
    }
}

void Logger::logTransfer(int threadId, int fromAccountId, int toAccountId,
                        double amount, const string& status) {
    lock_guard<mutex> guard(logMutex);
    if (logFile.is_open()) {
        logFile << "[" << getCurrentTimestamp() << "] "
                << "Thread-" << threadId << " | "
                << "TRANSFER | "
                << "From Account-" << fromAccountId 
                << " To Account-" << toAccountId << " | "
                << "Amount: $" << fixed << setprecision(2) << amount << " | "
                << "Status: " << status << endl;
    }
}

void Logger::logMessage(const string& message) {
    lock_guard<mutex> guard(logMutex);
    if (logFile.is_open()) {
        logFile << "[" << getCurrentTimestamp() << "] " << message << endl;
    }
}

void Logger::flush() {
    lock_guard<mutex> guard(logMutex);
    if (logFile.is_open()) {
        logFile.flush();
    }
}