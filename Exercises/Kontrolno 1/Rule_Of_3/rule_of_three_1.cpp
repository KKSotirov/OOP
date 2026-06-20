#include <iostream>
#include <stdexcept>
#include <cstring>
class Song
{
private:
    char *title;
    char *artist;
    int duration = 0;

    bool isNullPtr(const char *str) const
    {
        return str == nullptr;
    }

    size_t getSize(const char *str)
    {
        return strlen(str);
    }

    void initialize(const Song &other)
    {
        if (isNullPtr(other.artist) == false)
        {
            this->artist = new char[getSize(other.artist) + 1];
            strcpy(this->artist, other.artist);
        }
        else
            this->artist = nullptr;
        if (isNullPtr(other.title) == false)
        {
            this->title = new char[getSize(other.title) + 1];
            strcpy(this->title, other.title);
        }
        else
            this->title = nullptr;
        if (other.duration != 0)
            this->duration = other.duration;
    }

    void free(const Song &song)
    {
        delete[] song.artist;
        delete[] song.title;
        duration = 0;
    }

    size_t getSize(const Song &song) const
    {
        return strlen(song.artist);
    }

    void printSong(const Song &song)
    {
        Song tmpSong = song;
        for (size_t i = 0; i < getSize(tmpSong); i++)
        {
            std::cout << tmpSong.title[i];
        }
    }

public:
    Song() : title(nullptr), artist(nullptr), duration(0) {}

    Song(char *titlee, char *artistt, int durationn)
    {
        if (isNullPtr(titlee) || isNullPtr(artistt) || durationn == 0)
            throw std::invalid_argument("Invalid data for parametirized constructor!");
        this->title = new char[getSize(titlee) + 1];
        strcpy(this->title, titlee);
        this->artist = new char[getSize(artistt) + 1];
        strcpy(this->artist, artistt);
        this->duration = durationn;
    }

    // THE BIG 4 = DEF CONSTR + COPY CONSTR + OPERATOR= + DESTRUCTOR
    Song(const Song &other)
    {
        initialize(other);
    }

    Song &operator=(const Song &other)
    {
        if (this != &other)
        {
            free(*this);
            initialize(other);
        }
        return *this;
    }

    ~Song()
    {
        free(*this);
    }

    bool operator==(const Song &other)
    {
        if (isNullPtr(this->artist) && isNullPtr(other.artist) && this->duration == other.duration)
            return true;

        if (isNullPtr(this->title) || isNullPtr(other.title) || this->duration == other.duration)
            return false;

        return (strcmp(this->artist, other.artist) == 0 && strcmp(this->title, other.title) == 0 && this->duration == other.duration);
    }

    bool operator<(const Song &other)
    {
        // Оператор < за сравнение на две песни: по-малка е тази с по-малко времетраене. При еднакво времетраене, сравнявай по дължина на низа на изпълнителя (artist).
        if (this->duration == other.duration)
        {
            return (getSize(*this) < getSize(other));
        }

        return this->duration < other.duration;
    }

    void print()
    {
        std::cout << "The song is: ";
        printSong(*this);
    }

    int getDuration(const Song &other)
    {
        return other.duration;
    }
};

class Playlist
{
private:
    Song *songs;
    int *playCounts;
    const int maxDuration;

    size_t getSize(const Playlist &playlist)
    {
        // while --> songs[i].duration == 0
        //  number of songs
        size_t size = 0;
        const Song *tmpSongs = playlist.songs;
        while (true)
        {
            // if (Song::getDuration(tmpSongs[size]) == 0)
        }
    }

public:
};