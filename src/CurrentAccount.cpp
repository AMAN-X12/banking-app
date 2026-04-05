#include "CurrentAccount.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

CurrentAccount::CurrentAccount(const std::string& accountNumber,
                               const std::string& ownerName,
                               double initialBalance,
                               double overdraftLimit,
                               double transactionFee)
    : Account(accountNumber, ownerName, initialBalance),
      overdraftLimit(overdraftLimit),
      transactionFee(transactionFee) {}

std::string CurrentAccount::getAccountType()   const { return "Current"; }
double      CurrentAccount::getOverdraftLimit() const { return overdraftLimit; }
double      CurrentAccount::getAvailableBalance() const {
    return balance + overdraftLimit;
}

void CurrentAccount::withdraw(double amount, const std::string& description) {
    if (amount <= 0)
        throw std::invalid_argument("Withdrawal amount must be greater than zero.");

    double totalDeduction = amount + transactionFee;

    if (totalDeduction > getAvailableBalance())
        throw std::runtime_error(
            "Amount exceeds available balance including overdraft limit.");

    balance -= totalDeduction;

    recordTransaction("WITHDRAWAL", amount, description);
    if (transactionFee > 0)
        recordTransaction("FEE", transactionFee, "Transaction Fee");
}

void CurrentAccount::printSummary() const {
    Account::printSummary();
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  Overdraft  : $" << overdraftLimit         << "\n"
              << "  Available  : $" << getAvailableBalance()  << "\n";
}
