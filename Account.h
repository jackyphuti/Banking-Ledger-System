#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>

class Account {
private:
    std::string accountNumber;
    std::string accountHolder;
    long long balanceCents;  // Using cents (integers) to avoid floating-point precision issues
    
public:
    // Constructor
    Account(const std::string& number, const std::string& holder, long long initialBalance = 0);
    
    // Getters
    std::string getAccountNumber() const;
    std::string getAccountHolder() const;
    long long getBalance() const;
    
    // Balance operations
    void deposit(long long amountCents);
    bool withdraw(long long amountCents);  // Returns false if insufficient funds
    
    // Used for atomic transactions
    void addBalance(long long amountCents);      // Internal method for rollback
    void subtractBalance(long long amountCents); // Internal method for rollback
};

#endif // ACCOUNT_H
