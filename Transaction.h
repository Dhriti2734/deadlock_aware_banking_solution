#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <chrono>

enum class TransactionType {
    DEPOSIT,
    WITHDRAW,
    TRANSFER
};

enum class TransactionStatus {
    PENDING,
    PROCESSING,
    SUCCESS,
    FAILED,
    WAITING,
    UNSAFE
};

enum class Priority {
    LOW = 0,
    MEDIUM = 1,
    HIGH = 2
};

class Transaction {
private:
    static int nextId;
    std::string transactionId;
    TransactionType type;
    int sourceAccountId;
    int destAccountId;
    double amount;
    TransactionStatus status;
    Priority priority;
    std::chrono::steady_clock::time_point creationTime;
    int waitCount;

public:
    Transaction(TransactionType type, int sourceId, int destId, double amt, Priority prio = Priority::MEDIUM);
    
    std::string getId() const;
    TransactionType getType() const;
    int getSourceAccountId() const;
    int getDestAccountId() const;
    double getAmount() const;
    TransactionStatus getStatus() const;
    Priority getPriority() const;
    int getWaitCount() const;
    std::chrono::steady_clock::time_point getCreationTime() const;
    
    void setStatus(TransactionStatus status);
    void incrementWaitCount();
    void increasePriority();
    
    std::string toString() const;
    std::string toLogString() const;
};

#endif // TRANSACTION_H