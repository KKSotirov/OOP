#include "BankAccount.h"
#include <iostream>
#include <cstring>

void BankAccount::free()
{
    delete[] iban;
    iban = nullptr;
    delete[] ownerName;
    ownerName = nullptr;
}

void BankAccount::setterHelper(char *&dest, const char *src)
{
    char *tmp = src ? new char[strlen(src) + 1] : nullptr;
    if (src)
        strcpy(tmp, src);
    delete[] dest;
    dest = tmp;
}

BankAccount::BankAccount() : ownerName(nullptr), iban(nullptr), balance(0) {}

BankAccount::BankAccount(const char *_ownerName, const char *_iban, const double _balance) : ownerName(nullptr), iban(nullptr), balance(_balance)
{
    setIban(_iban);
    setOwnerName(_ownerName);
}

BankAccount::BankAccount(const BankAccount &other) : ownerName(nullptr), iban(nullptr)
{
    setOwnerName(other.ownerName);
    setIban(other.iban);
    setBalance(other.balance);
}

BankAccount &BankAccount::operator=(const BankAccount &other)
{
    if (this != &other)
    {
        setterHelper(iban, other.iban);
        setterHelper(ownerName, other.ownerName);
        balance = other.balance;
    }
    return *this;
}

BankAccount::~BankAccount()
{
    free();
}

// Getters + Setters
const char *BankAccount::getOwnerName() const
{
    return ownerName;
}

void BankAccount::setOwnerName(const char *_ownerName)
{
    setterHelper(ownerName, _ownerName);
}

const char *BankAccount::getIban() const
{
    return iban;
}

void BankAccount::setIban(const char *_iban)
{
    setterHelper(iban, _iban);
}

double BankAccount::getBalance() const
{
    return balance;
}

void BankAccount::setBalance(const double _balance)
{
    balance = _balance;
}

// Specific functions
void BankAccount::deposit(const double amount)
{
    if (amount > 0)
        balance += amount;
}

bool BankAccount::withdraw(const double amount)
{
    if (amount > 0 && balance >= amount)
    {
        balance -= amount;
        return true;
    }
    return false;
}

void BankAccount::printInfo() const
{
    std::cout << "Owner name : " << ownerName
              << ", IBAN = " << iban
              << ", Balance = " << balance
              << "\n";
}
