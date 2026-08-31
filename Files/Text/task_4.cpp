#include <iostream>
#include <fstream>
#include <cstring>

struct Song
{
    char *title;
    int durationSec;
    double rating;

    void init(const char *newTitle, const int newDuration, const double newRating)
    {
        title = new char[strlen(newTitle) + 1];
        strcpy(title, newTitle);
        durationSec = newDuration;
        rating = newRating;
    }

    void destroy()
    {
        delete[] title;
        title = nullptr;
    }

    bool writeToFile(std::ofstream &out)
    {
        if (!out)
        {
            std::cerr << "Error opening output file! \n";
            return false;
        }
        out << title << "|"
            << durationSec << "|"
            << rating << "\n";

        return true;
    }

    bool readFromFile(std::ifstream &in)
    {
        if (!in)
        {
            std::cerr << " Error opening input file! \n";
            return false;
        }
        char tmp[1024];
        in.getline(tmp, sizeof(tmp), '|');

        delete[] title;
        title = new char[strlen(tmp) + 1];
        strcpy(title, tmp);

        char seperator;
        in >> durationSec;
        in >> seperator;
        in >> rating;

        in.ignore(1000, '\n');
        return !in.fail();
    }
};