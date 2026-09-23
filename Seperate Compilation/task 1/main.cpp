#include "BankAccount.h"
#include <iostream>

int main()
{
    BankAccount pesho("Pesho Armianov", "TheB3st", 100000);
    // pesho creates a program for Google, optimising their software greatly, thus Google writes him a check
    pesho.deposit(5000000);
    // pesho donates 3 000 000€ for new FMI facilities
    if (pesho.withdraw(3000000))
        std::cout << "Pesho has successfully withdrawn money from his personal bank account in order to finance FMI!  \n";
    else
        std::cout << "Unfortunately Pesho does not currently have 3 000 000 euro  \n";

    // pesho wants to found a quantum academy of his own for 10 000 000€, but forgets he does not have that kind of money
    if (pesho.withdraw(10000000))
        std::cout << "Pesho has successfully withdrawn money from his personal bank account in order to build Pesho's Quantum Computing Academy!  \n";
    else
        std::cout << "Unfortunately Pesho does not currently have 10 000 000 euro  \n";
}