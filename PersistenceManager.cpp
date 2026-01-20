#include "PersistenceManager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>

PersistenceManager::PersistenceManager(const std::string& accountsFile,
                                       const std::string& transactionsFile)
    : accountsFilePath(accountsFile), transactionsFilePath(transactionsFile) {}

bool PersistenceManager::fileExists(const std::string& filePath) const {
    return std::filesystem::exists(filePath);
}

bool PersistenceManager::saveAccounts(const Ledger& /* ledger */) {
    std::ofstream file(accountsFilePath);
    if (!file.is_open()) {
        std::cerr << "Error opening " << accountsFilePath << " for writing." << std::endl;
        return false;
    }
    
    // Format: account_number|account_holder|balance_cents
    // This is a simplified version; in production, you'd use more robust serialization
    
    file << "# Account Data - DO NOT EDIT MANUALLY" << std::endl;
    file << "# Format: AccountNumber|HolderName|BalanceCents" << std::endl;
    
    // Note: In a real implementation, we'd need a method to iterate through accounts
    // For now, this is a placeholder showing the structure
    
    file.close();
    return true;
}

bool PersistenceManager::loadAccounts(Ledger& ledger) {
    if (!fileExists(accountsFilePath)) {
        std::cout << "No accounts file found. Starting with empty ledger." << std::endl;
        return true;
    }
    
    std::ifstream file(accountsFilePath);
    if (!file.is_open()) {
        std::cerr << "Error opening " << accountsFilePath << " for reading." << std::endl;
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        // Skip comments and empty lines
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        // Parse format: account_number|account_holder|balance_cents
        std::istringstream iss(line);
        std::string accNum, accHolder, balanceStr;
        
        if (std::getline(iss, accNum, '|') && 
            std::getline(iss, accHolder, '|') && 
            std::getline(iss, balanceStr)) {
            
            long long balance = std::stoll(balanceStr);
            ledger.createAccount(accNum, accHolder, balance);
        }
    }
    
    file.close();
    return true;
}

bool PersistenceManager::saveTransactions(const Ledger& ledger) {
    std::ofstream file(transactionsFilePath, std::ios::app);  // Append mode
    if (!file.is_open()) {
        std::cerr << "Error opening " << transactionsFilePath << " for writing." << std::endl;
        return false;
    }
    
    auto transactions = ledger.getTransactionHistory();
    
    for (const auto& txn : transactions) {
        file << txn.getFormattedString() << std::endl;
    }
    
    file.close();
    return true;
}

bool PersistenceManager::loadTransactions(Ledger& /* ledger */) {
    if (!fileExists(transactionsFilePath)) {
        std::cout << "No transaction history file found." << std::endl;
        return true;
    }
    
    std::ifstream file(transactionsFilePath);
    if (!file.is_open()) {
        std::cerr << "Error opening " << transactionsFilePath << " for reading." << std::endl;
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        // Transaction log is for display/audit purposes
        // In production, you'd deserialize and rebuild the ledger state
    }
    
    file.close();
    return true;
}
