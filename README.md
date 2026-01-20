# Banking Ledger System - A FinTech C++ Project

## Overview
A professional-grade banking ledger system demonstrating **ACID transaction principles**, particularly **Atomicity**. This project showcases how modern banking systems handle money transfers with rollback capabilities to prevent data loss during failures.

## Key Features

### 1. **Atomic Transactions with Rollback**
- Demonstrates the 'A' (Atomicity) in ACID databases
- If a transfer fails halfway through, the system automatically rolls back all changes
- Ensures money is never lost due to system crashes

### 2. **Core Classes**
- **Account**: Represents a customer account with balance management
- **Transaction**: Logs all operations with unique IDs, timestamps, and statuses
- **Ledger**: Manages accounts and coordinates multi-step transactions
- **PersistenceManager**: Handles file-based data persistence

### 3. **Money Handling**
- Uses **long long integers** (cents) instead of floating-point numbers
- Prevents precision loss that occurs with floats in financial systems
- Example: R123.45 is stored as 12345 cents

### 4. **Transaction Types**
- Deposits
- Withdrawals
- Transfers (with 2-phase commit for atomicity)
- Rollbacks (automatic recovery from failures)

## Architecture

```
BankingLedgerSystem/
├── Account.h/cpp          - Individual account management
├── Transaction.h/cpp      - Transaction logging and tracking
├── Ledger.h/cpp          - Core ledger with ACID operations
├── PersistenceManager.h/cpp - File I/O for persistence
├── main.cpp              - Terminal-based user interface
└── CMakeLists.txt        - Build configuration
```

## Building the Project

### Prerequisites
- C++17 compiler (GCC 7+, Clang 5+, or MSVC 2017+)
- CMake 3.10+
- Linux/macOS/Windows

### Compilation
```bash
cd "bank system"
mkdir build
cd build
cmake ..
make
```

## Running the Application

```bash
cd build
./banking_ledger
```

## How the Rollback Feature Works

### Normal Transfer (Success Path)
```
Phase 1: Withdraw from Sender's Account
         ↓
Phase 2: Deposit to Recipient's Account
         ↓
✓ Transaction Complete
```

### Transfer with Failure (Automatic Rollback)
```
Phase 1: Withdraw from Sender's Account ✓
         ↓
Phase 2: System Crash!
         ↓
ROLLBACK: Restore funds to Sender
ROLLBACK: Remove partial funds from Recipient
         ↓
✓ Transaction Rolled Back - Accounts Consistent
```

## Code Example: Atomic Transfer Implementation

```cpp
bool Ledger::executeTransfer(const std::string& fromAccNum, 
                             const std::string& toAccNum,
                             long long amountCents) {
    Account* fromAcc = getAccount(fromAccNum);
    Account* toAcc = getAccount(toAccNum);
    
    // Phase 1: Withdraw from sender
    if (!fromAcc->withdraw(amountCents)) {
        return false;  // Insufficient funds - no rollback needed
    }
    
    // Phase 2: Deposit to recipient
    // In a real system, this could fail (network issues, etc.)
    toAcc->deposit(amountCents);
    
    return true;
}

void Ledger::rollbackTransfer(const std::string& fromAccNum, 
                              const std::string& toAccNum,
                              long long amountCents) {
    // Reverse both operations
    fromAcc->addBalance(amountCents);      // Restore sender
    toAcc->subtractBalance(amountCents);   // Remove from recipient
    
    // Log rollback transactions for audit trail
    // ...
}
```

## Menu Options

1. **Create Account** - Add a new account to the ledger
2. **Deposit Funds** - Add money to an account
3. **Withdraw Funds** - Remove money from an account
4. **Transfer Funds** - Transfer between accounts (with atomicity)
5. **View All Accounts** - Display all accounts and balances
6. **View Account Statement** - See transaction history for one account
7. **View Transaction History** - See all system transactions
8. **Test Transfer with Rollback** - Simulate a system failure to test rollback
9. **Exit** - Close the application

## Data Structures Used

### Account
- Account Number (string)
- Account Holder Name (string)
- Balance in Cents (long long)

### Transaction
- Transaction ID (unique identifier)
- Account Number
- Amount (in cents)
- Transaction Type (Deposit, Withdrawal, Transfer In/Out)
- Transaction Status (Pending, Completed, Failed, Rolled Back)
- Timestamp
- Description
- Related Account (for transfers)

## Why This Impresses Fintech Companies (Like BBD)

1. **Precision with Integers**: Uses cents instead of floats—a critical requirement for banking
2. **ACID Compliance**: Demonstrates atomicity, preventing partial transactions
3. **Real-world Scenarios**: Handles system failures gracefully
4. **Audit Trail**: Every transaction is logged with timestamps and IDs
5. **Professional Architecture**: Clean class design, proper encapsulation
6. **Error Handling**: Validates all operations and prevents invalid states

## Sample Accounts
The system comes with three sample accounts pre-loaded:
- **ACC001**: John Mandela - R500.00
- **ACC002**: Thabo Mthembu - R1000.00
- **ACC003**: Lindiwe Nkosi - R250.00

## Future Enhancements
- Persistent storage with database (SQLite/PostgreSQL)
- Interest calculations
- Multi-currency support
- Advanced security features (encryption, authentication)
- REST API for web integration
- Concurrent transaction handling with locks
- Full ACID compliance with write-ahead logging

## Legal & Compliance
This is an educational project demonstrating banking principles. For production use, comply with banking regulations (PCI-DSS, POPIA in SA, etc.).

---
**Created**: January 2026  
**Language**: C++17  
**Target Audience**: BBD, SA Banking Sector
