// logger.h
// OS Concepts: File I/O, Thread-safe logging, Resource Management
#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>

using namespace std;

class Logger {
private:
    ofstream logFile;
    mutex logMutex;  // OS Concept: Ensure thread-safe file writes
    string filename;

    // Get current timestamp as string
    string getCurrentTimestamp() const;

public:
    // Constructor and Destructor
    Logger(const string& logFilename = "transaction_log.txt");
    ~Logger();

    // Prevent copying
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // OS Concept: Thread-safe logging operations
    void logTransaction(int threadId, const string& operation,
                       int accountId, double amount, const string& status);
    
    void logTransfer(int threadId, int fromAccountId, int toAccountId,
                    double amount, const string& status);
    
    void logMessage(const string& message);
    
    // Flush logs to disk
    void flush();
};

#endif // LOGGER_H