#ifndef PERSISTENCEMANAGER_H
#define PERSISTENCEMANAGER_H

#include "Ledger.h"
#include <string>

class PersistenceManager {
private:
    std::string accountsFilePath;
    std::string transactionsFilePath;
    
public:
    PersistenceManager(const std::string& accountsFile = "accounts.dat",
                      const std::string& transactionsFile = "transactions.log");
    
    // Save/Load operations
    bool saveAccounts(const Ledger& ledger);
    bool loadAccounts(Ledger& ledger);
    
    bool saveTransactions(const Ledger& ledger);
    bool loadTransactions(Ledger& ledger);
    
    // Utility
    bool fileExists(const std::string& filePath) const;
};

#endif // PERSISTENCEMANAGER_H
