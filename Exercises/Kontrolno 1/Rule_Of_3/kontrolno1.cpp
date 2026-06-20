#include <iostream>
#include <utility>
#include <cstring>
#include <stdexcept>

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

    // ЖИЗНЕН ЦИКЪЛ (Rule of 3)
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

    // ДОСТЪП ДО ДАННИТЕ (Getters & Setters)
    // Условието казва: "Не се очаква промяна на потребителското име", затова за email имаме само getter
    const char *getEmail() const
    {
        return this->email;
    }

    const char *getPassword() const
    {
        return this->password;
    }

    void setPassword(const char *newPassword)
    {
        if (newPassword == nullptr || strlen(newPassword) == 0)
            throw std::invalid_argument("Password cannot be empty!");

        char *temp = new char[strlen(newPassword) + 1];
        strcpy(temp, newPassword);

        delete[] this->password;
        this->password = temp;
    }

    // ПРЕДЕФИНИРАНИ ОПЕРАТОРИ
    bool operator==(const User &other) const
    {
        if (this->email == nullptr || other.email == nullptr)
            return false;
        return strcmp(this->email, other.email) == 0;
    }

    // Оператор за изход
    friend std::ostream &operator<<(std::ostream &os, const User &user)
    {
        os << (user.email != nullptr ? user.email : "NoEmail") << " "
           << (user.password != nullptr ? user.password : "NoPassword");
        return os;
    }

    // Оператор за вход (С класически стек-буфер за избягване на течове)
    friend std::istream &operator>>(std::istream &is, User &user)
    {
        char emBuf[1024];
        char pasBuf[1024];

        // Четем две последователни думи от потока
        if (is >> emBuf >> pasBuf)
        {
            // Проверяваме дали прочетеното е валидно според бизнес логиката ни
            if (user.isValidEmail(emBuf) && strlen(pasBuf) > 0)
            {
                // Използваме Copy-and-Swap тактиката локално за сигурност при изключения!
                User temp(emBuf, pasBuf);
                user.swap(temp);
            }
            else
            {
                is.setstate(std::ios::failbit); // Чупим потока, ако данните са невалидни
            }
        }
        return is;
    }
};

class Administrator : public User
{
private:
    unsigned yearsInternship;
    int competency; // 1 - 5

    bool validData(unsigned inter, int com) const
    {
        // inter >= 0 е винаги вярно, защото типът е unsigned, затова проверяваме само компетентността
        return (com >= 1 && com <= 5);
    }

public:
    // 1. Дефолтен конструктор - раждаме обекта в абсолютно валидно състояние
    Administrator() : User(), yearsInternship(0), competency(1) {}

    // 2. Параметризиран конструктор - задължително приема данните за User и ги препраща нагоре!
    Administrator(const char *em, const char *pas, unsigned years, int comp)
        : User(em, pas), yearsInternship(years), competency(comp)
    {
        if (!validData(years, comp))
            throw std::invalid_argument("Invalid administrator data! Competency must be between 1 and 5.");
    }

    // Тъй като нямаме нова динамична памет в този клас, няма нужда от Rule of 3!
    // Деструкторът и копирането на User ще си свършат работата автоматично.

    int getCompetence() const { return this->competency; }
    unsigned getYearsInternship() const { return this->yearsInternship; }

    // 3. Оператори за сравнение (Наредба - връщат bool!)
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
        return other > *this; // Чисто преизползване на оператор > без никаква рекурсия!
    }

    // 4. Оператори ++ и -- (Префиксни и Постфиксни) за промяна на компетентността
    // Префиксен (++admin)
    Administrator &operator++()
    {
        if (this->competency < 5)
            this->competency++;
        return *this;
    }

    // Постфиксен (admin++)
    Administrator operator++(int)
    {
        Administrator temp(*this);
        ++(*this); // викаме префиксния
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
        --(*this); // викаме префиксния
        return temp;
    }

    // 5. Потоци при Наследяване
    friend std::ostream &operator<<(std::ostream &os, const Administrator &other)
    {
        // Кастваме към "const User&", за да извикаме оператора на базовия клас за имейла и паролата
        os << static_cast<const User &>(other) << " "
           << other.yearsInternship << " " << other.competency;
        return os;
    }

    friend std::istream &operator>>(std::istream &is, Administrator &other)
    {
        // Първо четем базовата част (User) през нейния оператор
        is >> static_cast<User &>(other);

        if (is) // Ако базовата част е прочетена успешно, тогава четем специфичните за админа данни
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
                    is.setstate(std::ios::failbit); // Данните не са в границите -> чупим потока
                }
            }
        }
        return is;
    }
};