#include "Ledger.h"
#include <iostream>
#include <iomanip>

bool Ledger::createAccount(const std::string& accountNumber, const std::string& accountHolder,
                          long long initialBalanceCents) {
    if (accounts.find(accountNumber) != accounts.end()) {
        return false;  // Account already exists
    }
    
    accounts.emplace(accountNumber, Account(accountNumber, accountHolder, initialBalanceCents));
    return true;
}

bool Ledger::accountExists(const std::string& accountNumber) const {
    return accounts.find(accountNumber) != accounts.end();
}

Account* Ledger::getAccount(const std::string& accountNumber) {
    auto it = accounts.find(accountNumber);
    if (it != accounts.end()) {
        return &(it->second);
    }
    return nullptr;
}

const Account* Ledger::getAccount(const std::string& accountNumber) const {
    auto it = accounts.find(accountNumber);
    if (it != accounts.end()) {
        return &(it->second);
    }
    return nullptr;
}

bool Ledger::deposit(const std::string& accountNumber, long long amountCents, const std::string& reason) {
    Account* acc = getAccount(accountNumber);
    if (!acc || amountCents <= 0) {
        return false;
    }
    
    acc->deposit(amountCents);
    
    Transaction txn(accountNumber, amountCents, TransactionType::DEPOSIT, reason);
    txn.setStatus(TransactionStatus::COMPLETED);
    transactionHistory.push_back(txn);
    
    return true;
}

bool Ledger::withdrawal(const std::string& accountNumber, long long amountCents, const std::string& reason) {
    Account* acc = getAccount(accountNumber);
    if (!acc || amountCents <= 0) {
        return false;
    }
    
    if (acc->withdraw(amountCents)) {
        Transaction txn(accountNumber, amountCents, TransactionType::WITHDRAWAL, reason);
        txn.setStatus(TransactionStatus::COMPLETED);
        transactionHistory.push_back(txn);
        return true;
    }
    
    // Withdrawal failed - log as failed transaction
    Transaction txn(accountNumber, amountCents, TransactionType::WITHDRAWAL, reason);
    txn.setStatus(TransactionStatus::FAILED);
    transactionHistory.push_back(txn);
    return false;
}

bool Ledger::executeTransfer(const std::string& fromAccNum, const std::string& toAccNum,
                            long long amountCents) {
    Account* fromAcc = getAccount(fromAccNum);
    Account* toAcc = getAccount(toAccNum);
    
    if (!fromAcc || !toAcc || amountCents <= 0) {
        return false;
    }
    
    // Phase 1: Withdraw from sender
    if (!fromAcc->withdraw(amountCents)) {
        return false;  // Insufficient funds
    }
    
    // Phase 2: Deposit to recipient
    // In a real system, this could fail (network issues, etc.)
    toAcc->deposit(amountCents);
    
    return true;
}

void Ledger::rollbackTransfer(const std::string& fromAccNum, const std::string& toAccNum,
                             long long amountCents) {
    Account* fromAcc = getAccount(fromAccNum);
    Account* toAcc = getAccount(toAccNum);
    
    if (fromAcc && toAcc) {
        // Reverse the operations
        fromAcc->addBalance(amountCents);        // Restore to sender
        toAcc->subtractBalance(amountCents);     // Remove from recipient
        
        // Log rollback transactions
        Transaction rollbackOut(fromAccNum, amountCents, TransactionType::ROLLBACK_DEPOSIT,
                                "Rollback from failed transfer to " + toAccNum);
        rollbackOut.setStatus(TransactionStatus::COMPLETED);
        transactionHistory.push_back(rollbackOut);
        
        Transaction rollbackIn(toAccNum, amountCents, TransactionType::ROLLBACK_WITHDRAWAL,
                              "Rollback from failed transfer from " + fromAccNum);
        rollbackIn.setStatus(TransactionStatus::COMPLETED);
        transactionHistory.push_back(rollbackIn);
    }
}

bool Ledger::transfer(const std::string& fromAccNum, const std::string& toAccNum,
                      long long amountCents, const std::string& reason) {
    if (!accountExists(fromAccNum) || !accountExists(toAccNum)) {
        return false;
    }
    
    // Log outgoing transfer
    Transaction txnOut(fromAccNum, amountCents, TransactionType::TRANSFER_OUT, reason, toAccNum);
    
    // Log incoming transfer
    Transaction txnIn(toAccNum, amountCents, TransactionType::TRANSFER_IN, reason, fromAccNum);
    
    // Attempt the atomic transfer
    if (executeTransfer(fromAccNum, toAccNum, amountCents)) {
        txnOut.setStatus(TransactionStatus::COMPLETED);
        txnIn.setStatus(TransactionStatus::COMPLETED);
        transactionHistory.push_back(txnOut);
        transactionHistory.push_back(txnIn);
        return true;
    } else {
        // Transfer failed - mark transactions as failed and perform rollback
        txnOut.setStatus(TransactionStatus::FAILED);
        txnIn.setStatus(TransactionStatus::FAILED);
        transactionHistory.push_back(txnOut);
        transactionHistory.push_back(txnIn);
        
        // Ensure rollback (defensive programming)
        rollbackTransfer(fromAccNum, toAccNum, amountCents);
        return false;
    }
}

bool Ledger::transferWithFailureSimulation(const std::string& fromAccNum, const std::string& toAccNum,
                                          long long amountCents, bool failAtPhase2,
                                          const std::string& reason) {
    if (!accountExists(fromAccNum) || !accountExists(toAccNum)) {
        return false;
    }
    
    std::cout << "\n[SIMULATION] Starting transfer from " << fromAccNum << " to " << toAccNum << std::endl;
    
    Account* fromAcc = getAccount(fromAccNum);
    Account* toAcc = getAccount(toAccNum);
    
    // Phase 1: Withdraw
    std::cout << "[PHASE 1] Withdrawing R" << std::fixed << std::setprecision(2) << (amountCents / 100.0)
              << " from " << fromAccNum << "..." << std::endl;
    
    if (!fromAcc->withdraw(amountCents)) {
        std::cout << "[ERROR] Insufficient funds! Rollback not needed." << std::endl;
        return false;
    }
    std::cout << "[SUCCESS] Phase 1 complete." << std::endl;
    
    // Simulate system failure at Phase 2
    if (failAtPhase2) {
        std::cout << "[CRITICAL] System crash detected during Phase 2!" << std::endl;
        std::cout << "[ROLLBACK] Initiating automatic rollback..." << std::endl;
        
        // Rollback Phase 1
        fromAcc->addBalance(amountCents);
        std::cout << "[SUCCESS] Funds restored to " << fromAccNum << std::endl;
        
        Transaction txn(fromAccNum, amountCents, TransactionType::ROLLBACK_DEPOSIT,
                        "Automatic rollback due to system failure");
        txn.setStatus(TransactionStatus::COMPLETED);
        transactionHistory.push_back(txn);
        
        return false;
    }
    
    // Phase 2: Deposit
    std::cout << "[PHASE 2] Depositing R" << std::fixed << std::setprecision(2) << (amountCents / 100.0)
              << " to " << toAccNum << "..." << std::endl;
    toAcc->deposit(amountCents);
    std::cout << "[SUCCESS] Phase 2 complete. Transfer successful!" << std::endl;
    
    // Log transactions
    Transaction txnOut(fromAccNum, amountCents, TransactionType::TRANSFER_OUT, reason, toAccNum);
    txnOut.setStatus(TransactionStatus::COMPLETED);
    transactionHistory.push_back(txnOut);
    
    Transaction txnIn(toAccNum, amountCents, TransactionType::TRANSFER_IN, reason, fromAccNum);
    txnIn.setStatus(TransactionStatus::COMPLETED);
    transactionHistory.push_back(txnIn);
    
    return true;
}

std::vector<Transaction> Ledger::getTransactionHistory() const {
    return transactionHistory;
}

std::vector<Transaction> Ledger::getAccountTransactions(const std::string& accountNumber) const {
    std::vector<Transaction> accountTxns;
    
    for (const auto& txn : transactionHistory) {
        if (txn.getAccountNumber() == accountNumber) {
            accountTxns.push_back(txn);
        }
    }
    
    return accountTxns;
}

void Ledger::displayAllAccounts() const {
    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout << "ALL ACCOUNTS" << std::endl;
    std::cout << std::string(80, '=') << std::endl;
    
    if (accounts.empty()) {
        std::cout << "No accounts found." << std::endl;
        return;
    }
    
    std::cout << std::left << std::setw(15) << "Account #"
              << std::left << std::setw(25) << "Holder Name"
              << std::right << std::setw(20) << "Balance (R)" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    
    for (const auto& pair : accounts) {
        const Account& acc = pair.second;
        std::cout << std::left << std::setw(15) << acc.getAccountNumber()
                  << std::left << std::setw(25) << acc.getAccountHolder()
                  << std::right << std::setw(20) << std::fixed << std::setprecision(2)
                  << (acc.getBalance() / 100.0) << std::endl;
    }
    
    std::cout << std::string(80, '=') << std::endl;
}

void Ledger::displayAccountStatement(const std::string& accountNumber) const {
    const Account* acc = getAccount(accountNumber);
    if (!acc) {
        std::cout << "Account not found." << std::endl;
        return;
    }
    
    std::cout << "\n" << std::string(100, '=') << std::endl;
    std::cout << "ACCOUNT STATEMENT: " << accountNumber << std::endl;
    std::cout << "Holder: " << acc->getAccountHolder() << std::endl;
    std::cout << "Current Balance: R" << std::fixed << std::setprecision(2) << (acc->getBalance() / 100.0) << std::endl;
    std::cout << std::string(100, '=') << std::endl;
    
    auto txns = getAccountTransactions(accountNumber);
    if (txns.empty()) {
        std::cout << "No transactions found." << std::endl;
        return;
    }
    
    for (const auto& txn : txns) {
        std::cout << txn.getFormattedString() << std::endl;
    }
    
    std::cout << std::string(100, '=') << std::endl;
}

void Ledger::displayTransactionHistory() const {
    std::cout << "\n" << std::string(100, '=') << std::endl;
    std::cout << "COMPLETE TRANSACTION HISTORY" << std::endl;
    std::cout << std::string(100, '=') << std::endl;
    
    if (transactionHistory.empty()) {
        std::cout << "No transactions found." << std::endl;
        return;
    }
    
    for (const auto& txn : transactionHistory) {
        std::cout << txn.getFormattedString() << std::endl;
    }
    
    std::cout << std::string(100, '=') << std::endl;
}
