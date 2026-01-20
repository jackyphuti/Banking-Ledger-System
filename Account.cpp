#include "Account.h"

Account::Account(const std::string& number, const std::string& holder, long long initialBalance)
    : accountNumber(number), accountHolder(holder), balanceCents(initialBalance) {}

std::string Account::getAccountNumber() const {
    return accountNumber;
}

std::string Account::getAccountHolder() const {
    return accountHolder;
}

long long Account::getBalance() const {
    return balanceCents;
}

void Account::deposit(long long amountCents) {
    if (amountCents > 0) {
        balanceCents += amountCents;
    }
}

bool Account::withdraw(long long amountCents) {
    if (amountCents > 0 && balanceCents >= amountCents) {
        balanceCents -= amountCents;
        return true;
    }
    return false;
}

void Account::addBalance(long long amountCents) {
    balanceCents += amountCents;
}

void Account::subtractBalance(long long amountCents) {
    balanceCents -= amountCents;
}
