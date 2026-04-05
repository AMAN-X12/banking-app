#include "Account.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>


static std::string currentTimestamp() {
    std::time_t now = std::time(nullptr);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return std::string(buf);
}

Transaction::Transaction(const std::string& type, double amount,
                         double balanceAfter, const std::string& description)
    : type(type), amount(amount), balanceAfter(balanceAfter),
      timestamp(currentTimestamp()), description(description) {}


Account::Account(const std::string& accountNumber,
                 const std::string& ownerName,
                 double initialBalance)
    : accountNumber(accountNumber),
      ownerName(ownerName),
      balance(initialBalance),
      isActive(true) {
    if (initialBalance < 0)
        throw std::invalid_argument("Initial balance cannot be negative.");
}

std::string Account::getAccountNumber() const { return accountNumber; }
std::string Account::getOwnerName()     const { return ownerName; }
double      Account::getBalance()       const { return balance; }
bool        Account::getIsActive()      const { return isActive; }

const std::vector<Transaction>& Account::getTransactionHistory() const {
    return transactionHistory;
}

void Account::recordTransaction(const std::string& type, double amount,
                                const std::string& description) {
    transactionHistory.emplace_back(type, amount, balance, description);
}

void Account::deposit(double amount, const std::string& description) {
    if (amount <= 0)
        throw std::invalid_argument("Deposit amount must be greater than zero.");
    balance += amount;
    recordTransaction("DEPOSIT", amount, description);
}

void Account::withdraw(double amount, const std::string& description) {
    if (amount <= 0)
        throw std::invalid_argument("Withdrawal amount must be greater than zero.");
    if (amount > balance)
        throw std::runtime_error("Insufficient funds.");
    balance -= amount;
    recordTransaction("WITHDRAWAL", amount, description);
}

void Account::printSummary() const {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  Account No : " << accountNumber           << "\n"
              << "  Owner      : " << ownerName               << "\n"
              << "  Type       : " << getAccountType()        << "\n"
              << "  Balance    : $" << balance                << "\n"
              << "  Status     : " << (isActive ? "Active" : "Inactive") << "\n";
}

void Account::printStatement() const {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n  Transaction History — " << getAccountType() << " (" << accountNumber << ")\n";
    std::cout << "  " << std::string(72, '-') << "\n";
    std::cout << "  " << std::left
              << std::setw(20) << "Date/Time"
              << std::setw(15) << "Type"
              << std::setw(12) << "Amount"
              << std::setw(15) << "Balance"
              << "Description\n";
    std::cout << "  " << std::string(72, '-') << "\n";

    if (transactionHistory.empty()) {
        std::cout << "  No transactions recorded yet.\n";
        return;
    }

    for (const auto& t : transactionHistory) {
        std::cout << "  " << std::left
                  << std::setw(20) << t.timestamp
                  << std::setw(15) << t.type
                  << "$" << std::setw(11) << t.amount
                  << "$" << std::setw(14) << t.balanceAfter
                  << t.description << "\n";
    }
}
