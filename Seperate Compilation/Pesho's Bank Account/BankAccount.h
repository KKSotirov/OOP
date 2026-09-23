#pragma once

class BankAccount
{
private:
    char *ownerName;
    char *iban;
    double balance;

    void free();
    void setterHelper(char *&dest, const char *src);

public:
    // RO3
    BankAccount();
    BankAccount(const char *_ownerName, const char *_iban, const double _balance);
    BankAccount(const BankAccount &other);
    BankAccount &operator=(const BankAccount &other);
    ~BankAccount();

    // Getters + Setters
    const char *getOwnerName() const;
    void setOwnerName(const char *_ownerName);
    const char *getIban() const;
    void setIban(const char *_iban);
    double getBalance() const;
    void setBalance(const double _balance);

    // Specific functions
    void deposit(const double amount);
    bool withdraw(const double amount);
    void printInfo() const;
};