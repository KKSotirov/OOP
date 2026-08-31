#include <iostream>
#include <fstream>

int main()
{
    std::ofstream file("student.txt", std::ios::out);

    if (!file)
    {
        // file has not opened correctly
        std::cerr << "Error opening the file" << "\n";
        return 1;
    }

    file << "Kaloyan Sotirov \n"
         << "Information systems \n"
         << "OOP";

    file.close();

    if (file.fail())
    {
        std::cerr << "Write error! \n";
        return 1;
    }

    std::cout << "The file has been writen in successfully! \n";
    return 0;
}