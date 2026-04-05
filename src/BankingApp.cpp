#include "BankingApp.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <stdexcept>
#include <string>

#ifdef _WIN32
  #define CLEAR_CMD "cls"
#else
  #define CLEAR_CMD "clear"
#endif


BankingApp::BankingApp()
    : savingsAcc("SAV-001", "amaan khan", 47000.0),
      currentAcc("CUR-001", "amaan khan",  3000.0),
      isLoggedIn(false) {}


void BankingApp::clearScreen() const {
    system(CLEAR_CMD);
}

void BankingApp::printHeader(const std::string& title) const {
    const int WIDTH = 60;
    std::string border(WIDTH, '=');
    std::string pad((WIDTH - static_cast<int>(title.length()) - 2) / 2, ' ');

    std::cout << "\n  " << border << "\n"
              << "  " << pad << " " << title << " " << pad << "\n"
              << "  " << border << "\n\n";
}

void BankingApp::printFooter() const {
    std::cout << "\n  " << std::string(60, '-') << "\n"
              << "  Bank of QAU  |  Your Trusted Financial Partner\n"
              << "  " << std::string(60, '-') << "\n";
}

void BankingApp::pauseAndContinue() const {
    std::cout << "\n  Press ENTER to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

bool BankingApp::confirmAction(const std::string& message) const {
    char ans;
    std::cout << "\n  " << message << " (Y/N): ";
    std::cin >> ans;
    return (ans == 'Y' || ans == 'y');
}

double BankingApp::getValidAmount(const std::string& prompt, double maxAmount) const {
    double amount = 0.0;
    while (true) {
        std::cout << "  " << prompt << " (max $"
                  << std::fixed << std::setprecision(2) << maxAmount << "): $";
        if (std::cin >> amount && amount > 0 && amount <= maxAmount) {
            return amount;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  [!] Invalid amount. Enter a value between $0.01 and $"
                  << maxAmount << ".\n";
    }
}


bool BankingApp::handleLogin() {
    clearScreen();
    printHeader("SECURE LOGIN");

    if (security.isAccountLocked()) {
        std::cout << "  [!] Account is locked. Please reset via security question.\n";
        printFooter();
        pauseAndContinue();
        return false;
    }

    std::string id, password;
    std::cout << "  User ID  : ";
    std::cin  >> id;
    std::cout << "  Password : ";
    std::cin  >> password;

    try {
        if (security.authenticate(id, password)) {
            std::cout << "\n  [+] Login successful. Welcome, "
                      << currentAcc.getOwnerName() << "!\n";
            pauseAndContinue();
            return true;
        } else {
            std::cout << "\n  [!] Invalid credentials. "
                      << security.getRemainingAttempts()
                      << " attempt(s) remaining.\n";
        }
    } catch (const std::exception& e) {
        std::cout << "\n  [!] " << e.what() << "\n";
    }

    printFooter();
    pauseAndContinue();
    return false;
}


void BankingApp::handleTransfer(Account& from, Account& to,
                                const std::string& fromLabel,
                                const std::string& toLabel) {
    clearScreen();
    printHeader("FUND TRANSFER");

    std::cout << "  From : " << fromLabel << "  ($"
              << std::fixed << std::setprecision(2) << from.getBalance() << ")\n"
              << "  To   : " << toLabel   << "  ($" << to.getBalance()   << ")\n\n";

    double amount = getValidAmount("Enter transfer amount", from.getBalance());

    if (!confirmAction("Confirm transfer of $" + std::to_string(amount) + "?"))  {
        std::cout << "\n  Transfer cancelled.\n";
        pauseAndContinue();
        return;
    }

    try {
        from.withdraw(amount, "Transfer to " + toLabel);
        to.deposit  (amount, "Transfer from " + fromLabel);
        std::cout << "\n  [+] Transfer successful.\n"
                  << "      " << fromLabel << " balance : $" << from.getBalance() << "\n"
                  << "      " << toLabel   << " balance : $" << to.getBalance()   << "\n";
    } catch (const std::exception& e) {
        std::cout << "\n  [!] Transfer failed: " << e.what() << "\n";
    }
    pauseAndContinue();
}


void BankingApp::handleSavingsMenu() {
    while (true) {
        clearScreen();
        printHeader("SAVINGS ACCOUNT");
        savingsAcc.printSummary();
        std::cout << "\n  [1] Deposit from Current Account\n"
                  << "  [2] Withdraw to Current Account\n"
                  << "  [3] View Transaction History\n"
                  << "  [0] Back to Main Menu\n";
        printFooter();
        std::cout << "\n  Choice: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                handleTransfer(currentAcc, savingsAcc, "Current", "Savings");
                break;
            case 2:
                handleTransfer(savingsAcc, currentAcc, "Savings", "Current");
                break;
            case 3:
                clearScreen();
                printHeader("SAVINGS STATEMENT");
                savingsAcc.printStatement();
                pauseAndContinue();
                break;
            case 0:
                return;
            default:
                std::cout << "\n  [!] Invalid option.\n";
                pauseAndContinue();
        }
    }
}


void BankingApp::handleCurrentMenu() {
    while (true) {
        clearScreen();
        printHeader("CURRENT ACCOUNT");
        currentAcc.printSummary();
        std::cout << "\n  [1] Send Money (External Transfer)\n"
                  << "  [2] View Transaction History\n"
                  << "  [0] Back to Main Menu\n";
        printFooter();
        std::cout << "\n  Choice: ";

        int choice;
        std::cin >> choice;

        if (choice == 1) {
            clearScreen();
            printHeader("SEND MONEY");
            std::cout << "  Available Balance : $"
                      << std::fixed << std::setprecision(2)
                      << currentAcc.getAvailableBalance() << "\n\n";

            std::string recipientID;
            std::cout << "  Recipient Account ID : ";
            std::cin  >> recipientID;

            double amount = getValidAmount("Transfer amount",
                                           currentAcc.getAvailableBalance());

            if (confirmAction("Send $" + std::to_string(amount) +
                              " to account " + recipientID + "?")) {
                try {
                    currentAcc.withdraw(amount, "Transfer to " + recipientID);
                    std::cout << "\n  [+] Transfer of $" << amount
                              << " sent to " << recipientID << " successfully.\n"
                              << "      Remaining Balance: $"
                              << currentAcc.getBalance() << "\n";
                } catch (const std::exception& e) {
                    std::cout << "\n  [!] Error: " << e.what() << "\n";
                }
            } else {
                std::cout << "\n  Transfer cancelled.\n";
            }
            pauseAndContinue();

        } else if (choice == 2) {
            clearScreen();
            printHeader("CURRENT STATEMENT");
            currentAcc.printStatement();
            pauseAndContinue();

        } else if (choice == 0) {
            return;
        } else {
            std::cout << "\n  [!] Invalid option.\n";
            pauseAndContinue();
        }
    }
}


void BankingApp::handleSecurityMenu() {
    while (true) {
        clearScreen();
        printHeader("SECURITY SETTINGS");
        std::cout << "  [1] Change Password\n"
                  << "  [2] Reset Password via Security Question\n"
                  << "  [0] Back to Main Menu\n";
        printFooter();
        std::cout << "\n  Choice: ";

        int choice;
        std::cin >> choice;

        if (choice == 1) {
            clearScreen();
            printHeader("CHANGE PASSWORD");
            std::string current, newPass, confirm;
            std::cout << "  Current Password : ";  std::cin >> current;
            std::cout << "  New Password     : ";  std::cin >> newPass;
            std::cout << "  Confirm Password : ";  std::cin >> confirm;

            if (newPass != confirm) {
                std::cout << "\n  [!] Passwords do not match.\n";
            } else {
                try {
                    security.changePassword(current, newPass);
                    std::cout << "\n  [+] Password changed successfully.\n";
                } catch (const std::exception& e) {
                    std::cout << "\n  [!] " << e.what() << "\n";
                }
            }
            pauseAndContinue();

        } else if (choice == 2) {
            clearScreen();
            printHeader("RESET PASSWORD");
            std::cout << "  Security Question: " << Security::SECURITY_QUESTION << "\n\n";
            std::string answer, newPass, confirm;
            std::cout << "  Your Answer      : ";  std::cin >> answer;
            std::cout << "  New Password     : ";  std::cin >> newPass;
            std::cout << "  Confirm Password : ";  std::cin >> confirm;

            if (newPass != confirm) {
                std::cout << "\n  [!] Passwords do not match.\n";
            } else {
                try {
                    security.resetPasswordViaSecurityQuestion(answer, newPass);
                    std::cout << "\n  [+] Password reset and account unlocked.\n";
                } catch (const std::exception& e) {
                    std::cout << "\n  [!] " << e.what() << "\n";
                }
            }
            pauseAndContinue();

        } else if (choice == 0) {
            return;
        } else {
            std::cout << "\n  [!] Invalid option.\n";
            pauseAndContinue();
        }
    }
}


void BankingApp::handleMainMenu() {
    while (true) {
        clearScreen();
        printHeader("MAIN MENU  —  BANK OF QAU");

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "  Account Holder : " << currentAcc.getOwnerName()       << "\n"
                  << "  Savings Balance: $" << savingsAcc.getBalance()        << "\n"
                  << "  Current Balance: $" << currentAcc.getBalance()        << "\n\n";

        std::cout << "  [1] Savings Account\n"
                  << "  [2] Current Account\n"
                  << "  [3] Security Settings\n"
                  << "  [0] Logout\n";
        printFooter();
        std::cout << "\n  Choice: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1: handleSavingsMenu(); break;
            case 2: handleCurrentMenu(); break;
            case 3: handleSecurityMenu(); break;
            case 0:
                isLoggedIn = false;
                std::cout << "\n  [+] Logged out successfully. Goodbye!\n";
                pauseAndContinue();
                return;
            default:
                std::cout << "\n  [!] Invalid option.\n";
                pauseAndContinue();
        }
    }
}



void BankingApp::run() {
    while (true) {
        isLoggedIn = handleLogin();
        if (isLoggedIn)
            handleMainMenu();
        else if (!confirmAction("Would you like to try again?"))
            break;
    }
    clearScreen();
    std::cout << "\n  Thank you for using Bank of QAU. Goodbye!\n\n";
}
