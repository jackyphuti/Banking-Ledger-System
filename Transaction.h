#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <ctime>

enum class TransactionType {
    DEPOSIT,
    WITHDRAWAL,
    TRANSFER_OUT,
    TRANSFER_IN,
    ROLLBACK_WITHDRAWAL,
    ROLLBACK_DEPOSIT
};

enum class TransactionStatus {
    PENDING,
    COMPLETED,
    ROLLED_BACK,
    FAILED
};

class Transaction {
private:
    std::string transactionId;
    std::string accountNumber;
    std::string description;
    std::string relatedAccountNumber;  // For transfers: the other account involved
    long long amountCents;
    TransactionType type;
    std::time_t timestamp;
    TransactionStatus status;
    
public:
    // Constructor
    Transaction(const std::string& accNum, long long amount, TransactionType txnType,
                const std::string& desc = "", const std::string& relatedAcc = "");
    
    // Getters
    std::string getTransactionId() const;
    std::string getAccountNumber() const;
    long long getAmount() const;
    TransactionType getType() const;
    TransactionStatus getStatus() const;
    std::time_t getTimestamp() const;
    std::string getDescription() const;
    std::string getRelatedAccountNumber() const;
    
    // Setters
    void setStatus(TransactionStatus newStatus);
    
    // Utility
    std::string getFormattedString() const;
    static std::string statusToString(TransactionStatus status);
    static std::string typeToString(TransactionType type);
};

#endif // TRANSACTION_H
