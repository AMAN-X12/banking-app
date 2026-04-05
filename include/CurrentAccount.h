#pragma once
#include "Account.h"

class CurrentAccount : public Account {
private:
    double overdraftLimit;
    double transactionFee;

public:
    static constexpr double DEFAULT_OVERDRAFT = 10000.0;
    static constexpr double DEFAULT_FEE       = 0.0;

    CurrentAccount(const std::string& accountNumber,
                   const std::string& ownerName,
                   double initialBalance = 0.0,
                   double overdraftLimit = DEFAULT_OVERDRAFT,
                   double transactionFee = DEFAULT_FEE);

    void withdraw(double amount, const std::string& description = "Withdrawal") override;
    std::string getAccountType() const override;

    double getOverdraftLimit()    const;
    double getAvailableBalance()  const;
    void   printSummary()         const override;
};
