#include "Security.h"
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <cctype>

const std::string Security::SECURITY_QUESTION = "What university do you study at?";

std::string Security::hashString(const std::string& input) const {
    std::string lower = input;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    unsigned long hash = 5381;
    for (char c : lower)
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(c);

    return std::to_string(hash);
}

bool Security::validatePasswordStrength(const std::string& password) const {
    if (password.length() < 8) return false;

    bool hasUpper  = false, hasLower = false,
         hasDigit  = false, hasSpecial = false;

    for (char c : password) {
        if (std::isupper(c))  hasUpper   = true;
        if (std::islower(c))  hasLower   = true;
        if (std::isdigit(c))  hasDigit   = true;
        if (std::ispunct(c))  hasSpecial = true;
    }
    return hasUpper && hasLower && hasDigit && hasSpecial;
}

Security::Security(const std::string& userID,
                   const std::string& password,
                   const std::string& securityAns)
    : userID(userID),
      passwordHash(hashString(password)),
      securityAnswer(hashString(securityAns)),
      failedAttempts(0),
      isLocked(false) {}

bool Security::isAccountLocked()    const { return isLocked; }
int  Security::getFailedAttempts()  const { return failedAttempts; }
int  Security::getRemainingAttempts() const {
    return MAX_ATTEMPTS - failedAttempts;
}

bool Security::authenticate(const std::string& id, const std::string& password) {
    if (isLocked)
        throw std::runtime_error(
            "Account is locked after " + std::to_string(MAX_ATTEMPTS) +
            " failed attempts. Use security question to reset.");

    if (id == userID && hashString(password) == passwordHash) {
        failedAttempts = 0;
        return true;
    }

    failedAttempts++;
    if (failedAttempts >= MAX_ATTEMPTS)
        isLocked = true;

    return false;
}

void Security::resetLockout() {
    failedAttempts = 0;
    isLocked       = false;
}

bool Security::changePassword(const std::string& currentPassword,
                               const std::string& newPassword) {
    if (hashString(currentPassword) != passwordHash)
        throw std::runtime_error("Current password is incorrect.");

    if (!validatePasswordStrength(newPassword))
        throw std::invalid_argument(
            "Password must be at least 8 characters and include "
            "uppercase, lowercase, a digit, and a special character.");

    if (hashString(newPassword) == passwordHash)
        throw std::invalid_argument("New password must differ from the current one.");

    passwordHash = hashString(newPassword);
    return true;
}

bool Security::resetPasswordViaSecurityQuestion(const std::string& answer,
                                                 const std::string& newPassword) {
    if (hashString(answer) != securityAnswer)
        throw std::runtime_error("Security answer is incorrect.");

    if (!validatePasswordStrength(newPassword))
        throw std::invalid_argument(
            "Password must be at least 8 characters and include "
            "uppercase, lowercase, a digit, and a special character.");

    passwordHash = hashString(newPassword);
    resetLockout();
    return true;
}
