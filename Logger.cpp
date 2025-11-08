#include "Logger.h"
#include <iostream>
#include <sstream>
#include <ctime>

Logger* Logger::instance = nullptr;
std::mutex Logger::logMutex;

Logger::Logger() : logFileName("transaction_log.txt") {
    logFile.open(logFileName, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Error: Could not open log file." << std::endl;
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

Logger* Logger::getInstance() {
    if (instance == nullptr) {
        std::lock_guard<std::mutex> lock(logMutex);
        if (instance == nullptr) {
            instance = new Logger();
        }
    }
    return instance;
}

void Logger::log(const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);
    
    if (logFile.is_open()) {
        time_t now = time(0);
        char timestamp[26];
        ctime_s(timestamp, sizeof(timestamp), &now);
        timestamp[24] = '\0'; // Remove newline
        
        logFile << "[" << timestamp << "] " << message << std::endl;
        logFile.flush();
    }
}

void Logger::logTransaction(const std::string& transactionLog) {
    log(transactionLog);
    std::cout << "> Logger: Wrote [" << transactionLog << "] to log file." << std::endl;
}

void Logger::clear() {
    std::lock_guard<std::mutex> lock(logMutex);
    logFile.close();
    logFile.open(logFileName, std::ios::trunc);
    if (logFile.is_open()) {
        std::cout << "> Log file cleared." << std::endl;
    }
}

std::string Logger::readLog() {
    std::lock_guard<std::mutex> lock(logMutex);
    std::ifstream readFile(logFileName);
    std::ostringstream buffer;
    
    if (readFile.is_open()) {
        buffer << readFile.rdbuf();
        readFile.close();
    }
    
    return buffer.str();
}