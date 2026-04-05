CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
SRCS     = src/Account.cpp src/SavingsAccount.cpp src/CurrentAccount.cpp \
           src/Security.cpp src/BankingApp.cpp src/main.cpp
TARGET   = banking_app

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)
