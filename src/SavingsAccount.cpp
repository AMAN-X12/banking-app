#include "SavingsAccount.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

SavingsAccount::SavingsAccount(const std::string& accountNumber,
                               const std::string& ownerName,
                               double initialBalance,
                               double interestRate)
    : Account(accountNumber, ownerName, initialBalance),
      interestRate(interestRate),
      withdrawalLimit(MAX_WITHDRAWALS),
      withdrawalsThisMonth(0) {}

std::string SavingsAccount::getAccountType() const { return "Savings"; }

double SavingsAccount::getInterestRate()         const { return interestRate; }
int    SavingsAccount::getRemainingWithdrawals() const {
    return withdrawalLimit - withdrawalsThisMonth;
}

void SavingsAccount::withdraw(double amount, const std::string& description) {
    if (amount <= 0)
        throw std::invalid_argument("Withdrawal amount must be greater than zero.");

    if (withdrawalsThisMonth >= withdrawalLimit)
        throw std::runtime_error(
            "Monthly withdrawal limit reached (" +
            std::to_string(MAX_WITHDRAWALS) + " withdrawals per month).");

    if ((balance - amount) < MIN_BALANCE)
        throw std::runtime_error(
            "Withdrawal would breach minimum balance requirement of $" +
            std::to_string(static_cast<int>(MIN_BALANCE)) + ".");

    balance -= amount;
    withdrawalsThisMonth++;
    recordTransaction("WITHDRAWAL", amount, description);
}

void SavingsAccount::applyMonthlyInterest() {
    double interest = balance * (interestRate / 12.0);
    balance += interest;
    recordTransaction("DEPOSIT", interest, "Monthly Interest Credit");
}

void SavingsAccount::resetMonthlyWithdrawals() {
    withdrawalsThisMonth = 0;
}

void SavingsAccount::printSummary() const {
    Account::printSummary();
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  Interest   : " << (interestRate * 100) << "% p.a.\n"
              << "  Withdrawals: " << withdrawalsThisMonth
              << " / " << withdrawalLimit << " used this month\n"
              << "  Min Balance: $" << MIN_BALANCE << "\n";
}
