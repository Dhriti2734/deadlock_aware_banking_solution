#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>

class Logger {
private:
    static Logger* instance;
    static std::mutex logMutex;
    std::ofstream logFile;
    std::string logFileName;
    
    Logger();
    ~Logger();
    
public:
    static Logger* getInstance();
    
    void log(const std::string& message);
    void logTransaction(const std::string& transactionLog);
    void clear();
    std::string readLog();
    
    // Delete copy constructor and assignment operator
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
};

#endif // LOGGER_H