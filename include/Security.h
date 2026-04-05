#pragma once
#include <string>

class Security {
private:
    std::string userID;
    std::string passwordHash;
    std::string securityAnswer;
    int         failedAttempts;
    bool        isLocked;

    static constexpr int MAX_ATTEMPTS = 3;

    std::string hashString(const std::string& input) const;
    bool        validatePasswordStrength(const std::string& password) const;

public:
    static const std::string SECURITY_QUESTION;

    Security(const std::string& userID       = "ADMIN",
             const std::string& password     = "Admin@123",
             const std::string& securityAns  = "QAU");

    bool authenticate(const std::string& id, const std::string& password);
    bool isAccountLocked() const;
    void resetLockout();

    bool changePassword(const std::string& currentPassword,
                        const std::string& newPassword);
    bool resetPasswordViaSecurityQuestion(const std::string& answer,
                                          const std::string& newPassword);

    int  getFailedAttempts()  const;
    int  getRemainingAttempts() const;
};
