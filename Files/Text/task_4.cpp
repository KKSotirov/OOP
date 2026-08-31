#include <iostream>
#include <fstream>
#include <cstring>

struct Song
{
    char *title = nullptr;
    int durationSec;
    double rating;

    void init(const char *newTitle, const int newDuration, const double newRating)
    {
        delete[] title;
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

void resize(Song *&songs, size_t &capacity, const size_t size)
{
    capacity *= 2;
    try
    {
        Song *tmp = new Song[capacity];
        for (size_t i = 0; i < size; i++)
        {
            strcpy(tmp[i].title, songs[i].title);
            tmp[i].durationSec = songs[i].durationSec;
            tmp[i].rating = songs[i].rating;

            songs[i].destroy();
        }
        songs = tmp;
    }
    catch (...)
    {
        throw;
    }
}

int main()
{
    // >> Song* songs
    // std::cout << longestword
    // free mem

    std::ifstream in("playlist.txt");
    if (!in)
    {
        std::cerr << "Error opening the file! \n";
        return 1;
    }

    size_t capacity = 2;
    size_t size = 0;
    Song *songs = new Song[capacity];
    Song currentSong;

    while (currentSong.readFromFile(in))
    {

        if (size == capacity)
        {
            resize(songs, capacity, size);
        }
        songs[size++] = currentSong;
        currentSong.title = nullptr;
    }

    in.close();
    if (size == 0)
    {
        std::cout << "The playlist is empty! \n";
        delete[] songs;
        return 0;
    }

    int posMax = 0;
    for (size_t i = 0; i < size; i++)
    {
        if (songs[i].durationSec > songs[posMax].durationSec)
            posMax = i;
    }

    std::cout << "Longest song is: " << songs[posMax].title << "\n";

    for (size_t i = 0; i < size; i++)
    {
        songs->destroy();
    }

    return 0;
}