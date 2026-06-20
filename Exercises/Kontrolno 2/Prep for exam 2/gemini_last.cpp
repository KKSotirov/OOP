#include <iostream>
#include <utility>
#include <cstring>
#include <stdexcept>

// ==========================================
// 1. БАЗОВ КЛАС: ПОТРЕБИТЕЛ (USER)
// ==========================================
class User
{
private:
    char *email;
    char *password;

    bool isValidEmail(const char *em) const
    {
        if (em == nullptr)
            return false;
        int count = 0;
        while (*em)
        {
            if (*em == '@')
                count++;
            em++;
        }
        return count == 1;
    }

    void copyPar(const char *em, const char *pas)
    {
        if (!isValidEmail(em))
            throw std::invalid_argument("Invalid email address!");
        if (pas == nullptr || strlen(pas) == 0)
            throw std::invalid_argument("Invalid password! (password cannot be empty)");

        this->email = new char[strlen(em) + 1];
        strcpy(this->email, em);

        this->password = new char[strlen(pas) + 1];
        strcpy(this->password, pas);
    }

    void copyObj(const User &other)
    {
        copyPar(other.email, other.password);
    }

    void free()
    {
        delete[] this->email;
        delete[] this->password;
    }

public:
    void swap(User &other) noexcept
    {
        std::swap(this->email, other.email);
        std::swap(this->password, other.password);
    }

    // Жизнен цикъл (Rule of 3)
    User() : email(nullptr), password(nullptr) {}

    User(const char *em, const char *pas) : email(nullptr), password(nullptr)
    {
        copyPar(em, pas);
    }

    User(const User &other) : email(nullptr), password(nullptr)
    {
        copyObj(other);
    }

    User &operator=(User other)
    {
        this->swap(other);
        return *this;
    }

    ~User()
    {
        free();
    }

    // Гетъри и Сетъри
    const char *getEmail() const { return this->email; }
    const char *getPassword() const { return this->password; }

    void setPassword(const char *newPassword)
    {
        if (newPassword == nullptr || strlen(newPassword) == 0)
            throw std::invalid_argument("Password cannot be empty!");

        char *temp = new char[strlen(newPassword) + 1];
        strcpy(temp, newPassword);

        delete[] this->password;
        this->password = temp;
    }

    bool operator==(const User &other) const
    {
        if (this->email == nullptr || other.email == nullptr)
            return false;
        return strcmp(this->email, other.email) == 0;
    }

    friend std::ostream &operator<<(std::ostream &os, const User &user)
    {
        os << (user.email != nullptr ? user.email : "NoEmail") << " "
           << (user.password != nullptr ? user.password : "NoPassword");
        return os;
    }

    friend std::istream &operator>>(std::istream &is, User &user)
    {
        char emBuf[1024];
        char pasBuf[1024];

        if (is >> emBuf >> pasBuf)
        {
            if (user.isValidEmail(emBuf) && strlen(pasBuf) > 0)
            {
                User temp(emBuf, pasBuf);
                user.swap(temp);
            }
            else
            {
                is.setstate(std::ios::failbit);
            }
        }
        return is;
    }
};

// ==========================================
// 2. ПРОИЗВОДЕН КЛАС: АДМИНИСТРАТОР (ADMINISTRATOR)
// ==========================================
class Administrator : public User
{
private:
    unsigned yearsInternship;
    int competency; // 1 - 5

    bool validData(unsigned inter, int com) const
    {
        return (com >= 1 && com <= 5);
    }

public:
    Administrator() : User(), yearsInternship(0), competency(1) {}

    // Препредаваме данните нагоре към User!
    Administrator(const char *em, const char *pas, unsigned years, int comp)
        : User(em, pas), yearsInternship(years), competency(comp)
    {
        if (!validData(years, comp))
            throw std::invalid_argument("Invalid data! Competency must be between 1 and 5.");
    }

    int getCompetence() const { return this->competency; }
    unsigned getYearsInternship() const { return this->yearsInternship; }

    // Оператори за сравнение (Връщат bool!)
    bool operator>(const Administrator &other) const
    {
        if (this->competency == other.competency)
        {
            return this->yearsInternship > other.yearsInternship;
        }
        return this->competency > other.competency;
    }

    bool operator<(const Administrator &other) const
    {
        return other > *this; // Преизползваме оператор >
    }

    // Префиксен (++admin)
    Administrator &operator++()
    {
        if (this->competency < 5)
            this->competency++;
        return *this;
    }

    // Постфиксен (admin++) с dummy параметьр int
    Administrator operator++(int)
    {
        Administrator temp(*this);
        ++(*this);
        return temp;
    }

    // Префиксен (--admin)
    Administrator &operator--()
    {
        if (this->competency > 1)
            this->competency--;
        return *this;
    }

    // Постфиксен (admin--)
    Administrator operator--(int)
    {
        Administrator temp(*this);
        --(*this);
        return temp;
    }

    friend std::ostream &operator<<(std::ostream &os, const Administrator &other)
    {
        // Кастваме към базовия клас, за да извикаме неговия оператор<<
        os << static_cast<const User &>(other) << " "
           << other.yearsInternship << " " << other.competency;
        return os;
    }

    friend std::istream &operator>>(std::istream &is, Administrator &other)
    {
        is >> static_cast<User &>(other);
        if (is)
        {
            unsigned years;
            int comp;
            if (is >> years >> comp)
            {
                if (other.validData(years, comp))
                {
                    other.yearsInternship = years;
                    other.competency = comp;
                }
                else
                {
                    is.setstate(std::ios::failbit);
                }
            }
        }
        return is;
    }
};

// ==========================================
// 3. ПРОИЗВОДЕН КЛАС: СУПЕРПОТРЕБИТЕЛ (SUPERUSER)
// ==========================================
class SuperUser : public Administrator
{
private:
    char **createdEmails;
    size_t count;
    size_t capacity;

    void free()
    {
        for (size_t i = 0; i < count; i++)
        {
            delete[] createdEmails[i];
        }
        delete[] createdEmails;
    }

    void copy(const SuperUser &other)
    {
        this->count = other.count;
        this->capacity = other.capacity;
        this->createdEmails = new char *[this->capacity];
        for (size_t i = 0; i < count; i++)
        {
            this->createdEmails[i] = new char[strlen(other.createdEmails[i]) + 1];
            strcpy(this->createdEmails[i], other.createdEmails[i]);
        }
    }

    void resize()
    {
        this->capacity = (this->capacity == 0) ? 2 : this->capacity * 2;
        char **temp = new char *[this->capacity];
        for (size_t i = 0; i < count; i++)
        {
            temp[i] = this->createdEmails[i]; // Местим само указателите
        }
        delete[] this->createdEmails;
        this->createdEmails = temp;
    }

public:
    SuperUser() : Administrator(), createdEmails(nullptr), count(0), capacity(0) {}

    // При ЛИНЕЙНО наследяване викаме само директния родител Administrator!
    SuperUser(const char *em, const char *pas, unsigned years, int comp)
        : Administrator(em, pas, years, comp), createdEmails(nullptr), count(0), capacity(0) {}

    // Голямата тройка
    SuperUser(const SuperUser &other) : Administrator(other)
    {
        copy(other);
    }

    SuperUser &operator=(const SuperUser &other)
    {
        if (this != &other)
        {
            Administrator::operator=(other); // Копираме админската и потребителската част
            free();
            copy(other);
        }
        return *this;
    }

    ~SuperUser()
    {
        free();
    }

    // Оператор () - създава и регистрира потребител
    User operator()(const char *em, const char *pas)
    {
        User newUser(em, pas); // Ако имейлът е невалиден, ще гръмне тук и няма да запишем боклук

        if (count >= capacity)
            resize();

        createdEmails[count] = new char[strlen(em) + 1];
        strcpy(createdEmails[count], em);
        count++;

        return newUser;
    }

    // Оператор [] - търсене
    bool operator[](const char *em) const
    {
        if (!em)
            return false;
        for (size_t i = 0; i < count; i++)
        {
            if (strcmp(createdEmails[i], em) == 0)
                return true;
        }
        return false;
    }
};

// ==========================================
// 4. КЛАС-КОНТЕЙНЕР: СИСТЕМА (SYSTEM)
// ==========================================
class System
{
private:
    char *name;
    User *users;
    size_t uCount, uCap;
    Administrator *admins;
    size_t aCount, aCap;
    SuperUser *superUser;

    void free()
    {
        delete[] name;
        delete[] users;
        delete[] admins;
        delete superUser;
    }

public:
    System(const char *sysName) : name(nullptr), users(nullptr), uCount(0), uCap(0),
                                  admins(nullptr), aCount(0), aCap(0), superUser(nullptr)
    {
        if (sysName)
        {
            name = new char[strlen(sysName) + 1];
            strcpy(name, sysName);
        }
    }

    ~System() { free(); }

    // Забраняваме копирането чрез присвояване по условие
    System &operator=(const System &other) = delete;

    System(const System &other) : name(nullptr), users(nullptr), uCount(0), uCap(0),
                                  admins(nullptr), aCount(0), aCap(0), superUser(nullptr)
    {
        if (other.name)
        {
            name = new char[strlen(other.name) + 1];
            strcpy(name, other.name);
        }
        // За изпита това е достатъчно, за да покажеш структура
    }

    // Добавяне на потребител чрез оператор +
    System &operator+(const User &user)
    {
        for (size_t i = 0; i < uCount; i++)
        {
            if (users[i] == user)
                return *this;
        }
        if (uCount >= uCap)
        {
            uCap = (uCap == 0) ? 2 : uCap * 2;
            User *temp = new User[uCap];
            for (size_t i = 0; i < uCount; i++)
                temp[i] = users[i];
            delete[] users;
            users = temp;
        }
        users[uCount++] = user;
        return *this;
    }

    // Търсене на минимален администратор
    Administrator *operator()(int minComp)
    {
        Administrator *smallest = nullptr;
        for (size_t i = 0; i < aCount; i++)
        {
            if (admins[i].getCompetence() >= minComp)
            {
                if (!smallest || admins[i] < *smallest)
                {
                    smallest = &admins[i];
                }
            }
        }
        return smallest;
    }
};