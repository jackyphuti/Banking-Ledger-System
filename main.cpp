#include "Ledger.h"
#include <iostream>
#include <iomanip>
#include <limits>

void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void displayMainMenu() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "        BANKING LEDGER SYSTEM - MAIN MENU" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    std::cout << "1. Create Account" << std::endl;
    std::cout << "2. Deposit Funds" << std::endl;
    std::cout << "3. Withdraw Funds" << std::endl;
    std::cout << "4. Transfer Funds" << std::endl;
    std::cout << "5. View All Accounts" << std::endl;
    std::cout << "6. View Account Statement" << std::endl;
    std::cout << "7. View Transaction History" << std::endl;
    std::cout << "8. Test Transfer with Rollback" << std::endl;
    std::cout << "9. Exit" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    std::cout << "Enter your choice: ";
}

int main() {
    Ledger ledger;
    int choice;
    
    // Create sample accounts
    ledger.createAccount("ACC001", "John Mandela", 50000);      // R500
    ledger.createAccount("ACC002", "Thabo Mthembu", 100000);    // R1000
    ledger.createAccount("ACC003", "Lindiwe Nkosi", 25000);     // R250
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "  WELCOME TO BANKING LEDGER SYSTEM" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "\nSample accounts created for demonstration:" << std::endl;
    std::cout << "- ACC001: John Mandela (R500.00)" << std::endl;
    std::cout << "- ACC002: Thabo Mthembu (R1000.00)" << std::endl;
    std::cout << "- ACC003: Lindiwe Nkosi (R250.00)" << std::endl;
    
    while (true) {
        displayMainMenu();
        std::cin >> choice;
        clearInputBuffer();
        
        if (choice == 1) {
            // Create Account
            std::string accNum, accHolder;
            
            std::cout << "\n--- CREATE NEW ACCOUNT ---" << std::endl;
            std::cout << "Enter account number: ";
            std::getline(std::cin, accNum);
            
            std::cout << "Enter account holder name: ";
            std::getline(std::cin, accHolder);
            
            std::cout << "Enter initial balance (R): ";
            double balance;
            std::cin >> balance;
            clearInputBuffer();
            
            long long balanceCents = static_cast<long long>(balance * 100);
            
            if (ledger.createAccount(accNum, accHolder, balanceCents)) {
                std::cout << "\n✓ Account created successfully!" << std::endl;
            } else {
                std::cout << "\n✗ Account creation failed. Account may already exist." << std::endl;
            }
        }
        else if (choice == 2) {
            // Deposit Funds
            std::string accNum;
            double amount;
            
            std::cout << "\n--- DEPOSIT FUNDS ---" << std::endl;
            std::cout << "Enter account number: ";
            std::getline(std::cin, accNum);
            
            if (!ledger.accountExists(accNum)) {
                std::cout << "✗ Account not found." << std::endl;
                continue;
            }
            
            std::cout << "Enter amount (R): ";
            std::cin >> amount;
            clearInputBuffer();
            
            long long amountCents = static_cast<long long>(amount * 100);
            
            if (ledger.deposit(accNum, amountCents, "Cash deposit")) {
                std::cout << "\n✓ Deposit successful! New balance: R" << std::fixed << std::setprecision(2)
                          << (ledger.getAccount(accNum)->getBalance() / 100.0) << std::endl;
            } else {
                std::cout << "\n✗ Deposit failed." << std::endl;
            }
        }
        else if (choice == 3) {
            // Withdraw Funds
            std::string accNum;
            double amount;
            
            std::cout << "\n--- WITHDRAW FUNDS ---" << std::endl;
            std::cout << "Enter account number: ";
            std::getline(std::cin, accNum);
            
            if (!ledger.accountExists(accNum)) {
                std::cout << "✗ Account not found." << std::endl;
                continue;
            }
            
            std::cout << "Enter amount (R): ";
            std::cin >> amount;
            clearInputBuffer();
            
            long long amountCents = static_cast<long long>(amount * 100);
            
            if (ledger.withdrawal(accNum, amountCents, "Cash withdrawal")) {
                std::cout << "\n✓ Withdrawal successful! New balance: R" << std::fixed << std::setprecision(2)
                          << (ledger.getAccount(accNum)->getBalance() / 100.0) << std::endl;
            } else {
                std::cout << "\n✗ Withdrawal failed. Insufficient funds." << std::endl;
            }
        }
        else if (choice == 4) {
            // Transfer Funds
            std::string fromAcc, toAcc;
            double amount;
            
            std::cout << "\n--- TRANSFER FUNDS ---" << std::endl;
            std::cout << "Enter sender account number: ";
            std::getline(std::cin, fromAcc);
            
            std::cout << "Enter recipient account number: ";
            std::getline(std::cin, toAcc);
            
            if (!ledger.accountExists(fromAcc) || !ledger.accountExists(toAcc)) {
                std::cout << "✗ One or both accounts not found." << std::endl;
                continue;
            }
            
            std::cout << "Enter amount (R): ";
            std::cin >> amount;
            clearInputBuffer();
            
            long long amountCents = static_cast<long long>(amount * 100);
            
            if (ledger.transfer(fromAcc, toAcc, amountCents, "Inter-account transfer")) {
                std::cout << "\n✓ Transfer successful!" << std::endl;
                std::cout << "From balance: R" << std::fixed << std::setprecision(2)
                          << (ledger.getAccount(fromAcc)->getBalance() / 100.0) << std::endl;
                std::cout << "To balance: R" << std::fixed << std::setprecision(2)
                          << (ledger.getAccount(toAcc)->getBalance() / 100.0) << std::endl;
            } else {
                std::cout << "\n✗ Transfer failed. Check account balances." << std::endl;
            }
        }
        else if (choice == 5) {
            // View All Accounts
            ledger.displayAllAccounts();
        }
        else if (choice == 6) {
            // View Account Statement
            std::string accNum;
            std::cout << "\nEnter account number: ";
            std::getline(std::cin, accNum);
            
            ledger.displayAccountStatement(accNum);
        }
        else if (choice == 7) {
            // View Transaction History
            ledger.displayTransactionHistory();
        }
        else if (choice == 8) {
            // Test Transfer with Rollback
            std::string fromAcc, toAcc;
            double amount;
            
            std::cout << "\n--- TEST TRANSFER WITH ROLLBACK (ACID Demo) ---" << std::endl;
            std::cout << "This demonstrates atomicity: if Phase 2 fails, Phase 1 is rolled back." << std::endl;
            
            std::cout << "\nEnter sender account number: ";
            std::getline(std::cin, fromAcc);
            
            std::cout << "Enter recipient account number: ";
            std::getline(std::cin, toAcc);
            
            if (!ledger.accountExists(fromAcc) || !ledger.accountExists(toAcc)) {
                std::cout << "✗ One or both accounts not found." << std::endl;
                continue;
            }
            
            std::cout << "Enter amount (R): ";
            std::cin >> amount;
            clearInputBuffer();
            
            std::cout << "\nSimulate failure at Phase 2? (y/n): ";
            char failChoice;
            std::cin >> failChoice;
            clearInputBuffer();
            
            long long amountCents = static_cast<long long>(amount * 100);
            bool shouldFail = (failChoice == 'y' || failChoice == 'Y');
            
            std::cout << "\nBefore transfer:" << std::endl;
            std::cout << "  " << fromAcc << " balance: R" << std::fixed << std::setprecision(2)
                      << (ledger.getAccount(fromAcc)->getBalance() / 100.0) << std::endl;
            std::cout << "  " << toAcc << " balance: R" << std::fixed << std::setprecision(2)
                      << (ledger.getAccount(toAcc)->getBalance() / 100.0) << std::endl;
            
            ledger.transferWithFailureSimulation(fromAcc, toAcc, amountCents, shouldFail, "Test rollback");
            
            std::cout << "\nAfter transfer:" << std::endl;
            std::cout << "  " << fromAcc << " balance: R" << std::fixed << std::setprecision(2)
                      << (ledger.getAccount(fromAcc)->getBalance() / 100.0) << std::endl;
            std::cout << "  " << toAcc << " balance: R" << std::fixed << std::setprecision(2)
                      << (ledger.getAccount(toAcc)->getBalance() / 100.0) << std::endl;
        }
        else if (choice == 9) {
            std::cout << "\nThank you for using Banking Ledger System. Goodbye!" << std::endl;
            break;
        }
        else {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
    
    return 0;
}
