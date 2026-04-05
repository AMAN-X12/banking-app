#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <stdexcept>

struct Transaction {
    std::string type;       // "DEPOSIT", "WITHDRAWAL", "TRANSFER_IN", "TRANSFER_OUT"
    double amount;
    double balanceAfter;
    std::string timestamp;
    std::string description;

    Transaction(const std::string& type, double amount, double balanceAfter,
                const std::string& description);

};

class Account {
protected:
    std::string accountNumber;
    std::string ownerName;
    double balance;
    bool isActive;
    std::vector<Transaction> transactionHistory;

    void recordTransaction(const std::string& type, double amount,
                           const std::string& description);

public:
    Account(const std::string& accountNumber, const std::string& ownerName,
            double initialBalance = 0.0);
    virtual ~Account() = default;

    std::string getAccountNumber() const;
    std::string getOwnerName()     const;
    double      getBalance()       const;
    bool        getIsActive()      const;
    const std::vector<Transaction>& getTransactionHistory() const;

    virtual void deposit(double amount, const std::string& description = "Deposit");
    virtual void withdraw(double amount, const std::string& description = "Withdrawal");

    virtual void printStatement() const;
    virtual void printSummary()   const;
    virtual std::string getAccountType() const = 0;
};
