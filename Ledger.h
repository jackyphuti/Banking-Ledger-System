#ifndef LEDGER_H
#define LEDGER_H

#include "Account.h"
#include "Transaction.h"
#include <map>
#include <vector>
#include <memory>

class Ledger {
private:
    std::map<std::string, Account> accounts;
    std::vector<Transaction> transactionHistory;
    
    // Helper methods for rollback
    bool executeTransfer(const std::string& fromAccNum, const std::string& toAccNum,
                        long long amountCents);
    void rollbackTransfer(const std::string& fromAccNum, const std::string& toAccNum,
                         long long amountCents);
    
public:
    // Account management
    bool createAccount(const std::string& accountNumber, const std::string& accountHolder,
                       long long initialBalanceCents = 0);
    bool accountExists(const std::string& accountNumber) const;
    Account* getAccount(const std::string& accountNumber);
    const Account* getAccount(const std::string& accountNumber) const;
    
    // Transaction operations with ACID properties
    bool deposit(const std::string& accountNumber, long long amountCents, const std::string& reason = "");
    bool withdrawal(const std::string& accountNumber, long long amountCents, const std::string& reason = "");
    
    // ATOMIC TRANSFER with rollback capability
    bool transfer(const std::string& fromAccNum, const std::string& toAccNum,
                  long long amountCents, const std::string& reason = "");
    
    // Simulates a system failure during transfer (for testing rollback)
    bool transferWithFailureSimulation(const std::string& fromAccNum, const std::string& toAccNum,
                                       long long amountCents, bool failAtPhase2 = false,
                                       const std::string& reason = "");
    
    // Getters
    std::vector<Transaction> getTransactionHistory() const;
    std::vector<Transaction> getAccountTransactions(const std::string& accountNumber) const;
    
    // Display methods
    void displayAllAccounts() const;
    void displayAccountStatement(const std::string& accountNumber) const;
    void displayTransactionHistory() const;
};

#endif // LEDGER_H
