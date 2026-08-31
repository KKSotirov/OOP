#include <iostream>
#include <fstream>

int main()
{
    std::ifstream in("inventory.txt");
    if (!in)
    {
        std::cerr << "Error opening the input file! \n";
        return 1;
    }

    std::ofstream out("expensive.txt");
    if (!out)
    {
        std::cerr << "Error opening the output file! \n";
        return 1;
    }

    int countOfProducts;

    if (!(in >> countOfProducts))
    {
        std::cerr << "Error reading product count!\n";
        return 1;
    }

    int id;
    char name[128];
    double price;
    double totalPrice = 0.0;

    for (size_t i = 0; i < countOfProducts; i++)
    {
        in >> id;
        in.ignore(1000, '\n');
        in.getline(name, sizeof(name));
        in >> price;

        totalPrice += price;
    }
    double avgPrice = totalPrice / countOfProducts;

    // bringing the pointer back to the start
    in.clear();
    in.seekg(0, std::ios::beg);
    in >> countOfProducts;

    for (size_t i = 0; i < countOfProducts; i++)
    {
        in >> id;
        in.ignore(1000, '\n');
        in.getline(name, sizeof(name));
        in >> price;

        if (price > avgPrice)
        {
            out << id << ", "
                << name << ", "
                << price << "\n";
        }
    }

    std::cout << "Average price = " << avgPrice << "\n";
    std::cout << "Everything was executed perfectly! \n";

    in.close();
    out.close();
    return 0;
}
