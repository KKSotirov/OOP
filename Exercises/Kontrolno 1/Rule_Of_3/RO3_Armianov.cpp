#include <iostream>
#include <utility>
#include <cstring>
class User
{
private:
    char *username;
    char *ip_adress;
    int access_level; // ot 1 do 5

    void free()
    {
        delete[] this->username;
        this->username = nullptr;
        delete[] this->ip_adress;
        this->ip_adress = nullptr;
    }

    void swap(User &other) noexcept
    {
        // Armianov special <3
        std::swap(this->username, other.username);
        std::swap(this->ip_adress, other.ip_adress);
        std::swap(this->access_level, other.access_level);
    }

    bool isValidLevel(const int level) const
    {
        return (level >= 1 && level <= 5);
    }

    bool areCooked(const char *uName, const char *ip, const int level) const
    {
        return (uName == nullptr || ip == nullptr || isValidLevel(level) == false);
    }

    void copy(const User &other)
    {
        char *tmpUsername = nullptr;
        char *tmpAdress = nullptr;
        if (other.username != nullptr)
        {
            tmpUsername = new char[strlen(other.username) + 1];
            strcpy(tmpUsername, other.username);
        }
        try
        {
            if (other.ip_adress != nullptr)
            {
                tmpAdress = new char[strlen(other.ip_adress) + 1];
                strcpy(tmpAdress, other.ip_adress);
            }
        }
        catch (...)
        {
            delete[] tmpUsername;
            throw;
        }
        this->access_level = other.access_level;
        this->username = tmpUsername;
        this->ip_adress = tmpAdress;
    }

public:
    // Default constructor
    User() : username(nullptr), ip_adress(nullptr), access_level(1) {}

    // parameterized constructor
    User(const char *uName, const char *ip, const int level)
    {
        if (areCooked(uName, ip, level) == true)
            throw std::invalid_argument("Incorrect data in par. constructor!");
        // check for correct data

        char *tmpUsername = nullptr;
        char *tmpAdress = nullptr;
        if (uName != nullptr)
        {
            tmpUsername = new char[strlen(uName) + 1];
            strcpy(tmpUsername, uName);
        }
        try
        {
            if (ip != nullptr)
            {
                tmpAdress = new char[strlen(ip) + 1];
                strcpy(tmpAdress, ip);
            }
        }
        catch (...)
        {
            delete[] tmpUsername;
            throw;
        }
        this->access_level = level;
        this->username = tmpUsername;
        this->ip_adress = tmpAdress;
    }

    User(const User &other)
    {
        copy(other);
    }

    User &operator=(User other)
    {
        this->swap(other);
        return *this;
    }
    // User& operator=(const User& other) {
    //     if (this != &other)
    //     {
    //         free();
    //         copy(other);
    //     }
    // }

    ~User()
    {
        free();
    }
};