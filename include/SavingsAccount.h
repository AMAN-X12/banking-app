#pragma once
#include "Account.h"

class SavingsAccount : public Account {
private:
    double interestRate;        // Annual interest rate ( 5%)
    int    withdrawalLimit;
    int    withdrawalsThisMonth;

public:
    static constexpr double MIN_BALANCE      = 500.0;
    static constexpr int    MAX_WITHDRAWALS  = 6;

    SavingsAccount(const std::string& accountNumber,
                   const std::string& ownerName,
                   double initialBalance = 0.0,
                   double interestRate   = 0.05);

    void withdraw(double amount, const std::string& description = "Withdrawal") override;
    std::string getAccountType() const override;

    void   applyMonthlyInterest();
    void   resetMonthlyWithdrawals();
    double getInterestRate()         const;
    int    getRemainingWithdrawals() const;
    void   printSummary()            const override;
};
