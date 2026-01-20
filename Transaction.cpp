#include "Transaction.h"
#include <sstream>
#include <iomanip>

Transaction::Transaction(const std::string& accNum, long long amount, TransactionType txnType,
                         const std::string& desc, const std::string& relatedAcc)
    : transactionId(""), accountNumber(accNum), description(desc), relatedAccountNumber(relatedAcc),
      amountCents(amount), type(txnType), timestamp(std::time(nullptr)), status(TransactionStatus::PENDING) {
    
    // Generate unique transaction ID
    static int transactionCounter = 0;
    transactionId = "TXN" + std::to_string(++transactionCounter) + "_" + std::to_string(timestamp);
}

std::string Transaction::getTransactionId() const {
    return transactionId;
}

std::string Transaction::getAccountNumber() const {
    return accountNumber;
}

long long Transaction::getAmount() const {
    return amountCents;
}

TransactionType Transaction::getType() const {
    return type;
}

TransactionStatus Transaction::getStatus() const {
    return status;
}

std::time_t Transaction::getTimestamp() const {
    return timestamp;
}

std::string Transaction::getDescription() const {
    return description;
}

std::string Transaction::getRelatedAccountNumber() const {
    return relatedAccountNumber;
}

void Transaction::setStatus(TransactionStatus newStatus) {
    status = newStatus;
}

std::string Transaction::statusToString(TransactionStatus status) {
    switch (status) {
        case TransactionStatus::PENDING:
            return "PENDING";
        case TransactionStatus::COMPLETED:
            return "COMPLETED";
        case TransactionStatus::ROLLED_BACK:
            return "ROLLED_BACK";
        case TransactionStatus::FAILED:
            return "FAILED";
        default:
            return "UNKNOWN";
    }
}

std::string Transaction::typeToString(TransactionType type) {
    switch (type) {
        case TransactionType::DEPOSIT:
            return "DEPOSIT";
        case TransactionType::WITHDRAWAL:
            return "WITHDRAWAL";
        case TransactionType::TRANSFER_OUT:
            return "TRANSFER_OUT";
        case TransactionType::TRANSFER_IN:
            return "TRANSFER_IN";
        case TransactionType::ROLLBACK_WITHDRAWAL:
            return "ROLLBACK_WITHDRAWAL";
        case TransactionType::ROLLBACK_DEPOSIT:
            return "ROLLBACK_DEPOSIT";
        default:
            return "UNKNOWN";
    }
}

std::string Transaction::getFormattedString() const {
    std::ostringstream oss;
    
    // Convert timestamp to readable format
    struct tm* timeinfo = std::localtime(&timestamp);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    oss << std::fixed << std::setprecision(2);
    oss << "[" << buffer << "] ";
    oss << "ID: " << transactionId << " | ";
    oss << "Type: " << typeToString(type) << " | ";
    oss << "Amount: R" << (amountCents / 100.0) << " | ";
    oss << "Status: " << statusToString(status);
    
    if (!description.empty()) {
        oss << " | " << description;
    }
    
    return oss.str();
}
