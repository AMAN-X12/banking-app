

A robust, object-oriented Command Line Interface (CLI) banking application written in standard C++17. This system simulates a secure, multi-account banking environment with a focus on strict encapsulation, polymorphism, and defensive programming.

##  Features

* **Secure Authentication Engine:**
  * DJB2 cryptographic password hashing (no plain-text storage).
  * Strict password strength validation.
  * Anti-brute-force lockout mechanism (max 3 attempts).
  * Password reset via secure hashing of security questions.
* **Polymorphic Account Architecture:**
  * Abstract base `Account` class enforcing universal banking contracts.
  * **Current Accounts:** Supports overdraft limits and transaction fees.
  * **Savings Accounts:** Enforces monthly withdrawal limits, minimum balances, and applies monthly interest.
* **Transaction Ledger:**
  * Immutable, timestamped transaction history (`std::vector<Transaction>`) for every account action.
* **Interactive UI:**
  * Cross-platform clear screen handling.
  * Bulletproof input validation preventing infinite loops or type mismatches.

##  Prerequisites

Because this project uses the pure C++17 Standard Library, there are no external libraries to install. You only need:

* A C++ compiler that supports **C++17** (e.g., GCC via MinGW-w64 on Windows, or Clang/GCC on macOS/Linux).
* **GNU Make** (e.g., `mingw32-make` on Windows).
##  Build and Run Instructions

**Clone the repository:**
   ```bash
   git clone [https://github.com/yourusername/BankOfQAU.git](https://github.com/yourusername/BankOfQAU.git)
   cd BankOfQAU


## TO RUN
Run the make command to compile the source code into an executable.

On Windows (MinGW):

Bash
mingw32-make
On Linux/macOS:

Bash
make
Run the application:

On Windows:

Bash
.\banking_app.exe
On Linux/macOS:

Bash
./banking_app



