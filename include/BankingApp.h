#pragma once
#include "SavingsAccount.h"
#include "CurrentAccount.h"
#include "Security.h"
#include <string>

class BankingApp {
private:
    SavingsAccount savingsAcc;
    CurrentAccount currentAcc;
    Security       security;
    bool           isLoggedIn;

    void clearScreen()          const;
    void printHeader(const std::string& title) const;
    void printFooter()          const;
    void pauseAndContinue()     const;
    bool confirmAction(const std::string& message) const;
    double getValidAmount(const std::string& prompt, double maxAmount) const;

    bool handleLogin();
    void handleMainMenu();
    void handleSavingsMenu();
    void handleCurrentMenu();
    void handleSecurityMenu();
    void handleTransfer(Account& from, Account& to,
                        const std::string& fromLabel,
                        const std::string& toLabel);

public:
    BankingApp();
    void run();
};
