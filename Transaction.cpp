#include "Transaction.h"
#include <sstream>
#include <iomanip>

int Transaction::nextId = 1;

Transaction::Transaction(TransactionType type, int sourceId, int destId, double amt, Priority prio)
    : type(type), sourceAccountId(sourceId), destAccountId(destId), 
      amount(amt), status(TransactionStatus::PENDING), priority(prio),
      creationTime(std::chrono::steady_clock::now()), waitCount(0) {
    std::ostringstream oss;
    oss << "TID-" << std::setw(3) << std::setfill('0') << nextId++;
    transactionId = oss.str();
}

std::string Transaction::getId() const { return transactionId; }
TransactionType Transaction::getType() const { return type; }
int Transaction::getSourceAccountId() const { return sourceAccountId; }
int Transaction::getDestAccountId() const { return destAccountId; }
double Transaction::getAmount() const { return amount; }
TransactionStatus Transaction::getStatus() const { return status; }
Priority Transaction::getPriority() const { return priority; }
int Transaction::getWaitCount() const { return waitCount; }
std::chrono::steady_clock::time_point Transaction::getCreationTime() const { return creationTime; }

void Transaction::setStatus(TransactionStatus s) { status = s; }
void Transaction::incrementWaitCount() { waitCount++; }

void Transaction::increasePriority() {
    if (priority == Priority::LOW) priority = Priority::MEDIUM;
    else if (priority == Priority::MEDIUM) priority = Priority::HIGH;
}

std::string Transaction::toString() const {
    std::ostringstream oss;
    oss << "[" << transactionId << "] ";
    
    if (type == TransactionType::DEPOSIT)
        oss << "Deposit $" << std::fixed << std::setprecision(2) << amount 
            << " to " << sourceAccountId;
    else if (type == TransactionType::WITHDRAW)
        oss << "Withdraw $" << std::fixed << std::setprecision(2) << amount 
            << " from " << sourceAccountId;
    else
        oss << "Transfer $" << std::fixed << std::setprecision(2) << amount 
            << " from " << sourceAccountId << " to " << destAccountId;
    
    return oss.str();
}

std::string Transaction::toLogString() const {
    std::ostringstream oss;
    oss << transactionId << " | ";
    
    if (type == TransactionType::TRANSFER)
        oss << sourceAccountId << " -> " << destAccountId;
    else
        oss << sourceAccountId;
    
    oss << " | $" << std::fixed << std::setprecision(2) << amount << " | STATUS: ";
    
    switch(status) {
        case TransactionStatus::SUCCESS: oss << "SUCCESS"; break;
        case TransactionStatus::FAILED: oss << "FAILED"; break;
        case TransactionStatus::WAITING: oss << "WAITING"; break;
        case TransactionStatus::UNSAFE: oss << "UNSAFE"; break;
        default: oss << "PENDING";
    }
    
    return oss.str();
}