#include <iostream>
#include <cstring>
#include <stdexcept>
#include <utility>
const size_t MAX_INVENTORY_SIZE = 6;

class Item
{
private:
    int bonusAD;
    int bonusAP;
    int bonusArmor;
    int bonusMR;

public:
    Item &operator+(const Item &other)
    {
        this->bonusAD += other.bonusAD;
        this->bonusAP += other.bonusAP;
        this->bonusMR += other.bonusMR;
        this->bonusArmor += other.bonusArmor;
    }

    Item &operator+=(const Champion &other)
    {
    }
};

class Champion
{
private:
    char *name;
    const int id;
    static int counter;
    Item *items[MAX_INVENTORY_SIZE];

    void free()
    {
        delete[] this->name;
    }

protected:
    void setterHelper(char *&dest, const char *src)
    {
        char *temp = src ? new char[strlen(src) + 1] : nullptr;
        if (src != nullptr)
            strcpy(temp, src);
        delete[] dest;
        dest = temp;
    }

    void swap(Champion &other) noexcept
    {
        std::swap(this->name, other.name);
        // id's cannot be copied
    }

public:
    // Dyn Mem ==> RO3
    // Def constr
    Champion() : name(nullptr), id(++counter) {}
    // Par constr
    Champion(const char *newName) : name(nullptr), id(++counter)
    {
        setterHelper(this->name, newName);
    }
    // Copy constructor
    Champion(const Champion &other) : id(++counter), name(nullptr)
    {
        setterHelper(this->name, other.name);
    }
    // Operator =
    Champion &operator=(Champion other)
    {
        this->swap(other);
        return *this;
    }
    // Destructor
    ~Champion()
    {
        free();
    }

    // Operator >
    bool operator>(const Champion &other) const
    {
        return this->id > other.id;
    }
    bool operator<(const Champion &other) const
    {
        return other > *this;
    }
    friend std::ostream &operator<<(std::ostream &os, const Champion &object)
    {
        std::cout << "[KEY INFO] ~> {ID = " << object.id
                  << "},  {NAME = " << object.name << " }"
                  << std::endl;
    }
    virtual Champion *operator+(const Champion *other) = 0;
    virtual Champion *operator*(const int multiplier) const = 0;
    virtual Champion *clone() const = 0;
    virtual Champion *activate() const = 0; // use ability
    virtual Champion &operator++() = 0;     // Level up
};

int Champion::counter = 0;

class PhysicalFighter : virtual public Champion
{
private:
    int attackDamage;
};