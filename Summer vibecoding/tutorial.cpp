#include <iostream>
#include <utility>
#include <cstring>

class Item
{
private:
    char *name;
    int bonusHealth;
    int bonusAttack;

    void free()
    {
        delete[] name;
    }

protected:
    void setterHelper(char *dest, const char *src)
    {
        char *temp = src ? new char[strlen(src) + 1] : nullptr;
        if (src != nullptr)
            strcpy(temp, src);
        delete[] dest;
        dest = temp;
    }

    void swap(Item &other)
    {
        std::swap(this->bonusAttack, other.bonusAttack);
        std::swap(this->bonusHealth, other.bonusHealth);
        std::swap(this->name, other.name);
    }

public:
    // default constructor
    Item() : name(nullptr), bonusHealth(0), bonusAttack(0) {}
    // parametrised constructor
    Item(const char *str, const int bHealth, const int bAttack) : bonusHealth(bHealth), bonusAttack(bAttack)
    {
        setterHelper(this->name, str);
    }
    // Operator =
    Item &operator=(Item other)
    {
        this->swap(other);
        return *this;
    }
    // virtual destructor
    virtual ~Item()
    {
        free();
    }

    // GETTERS + SETTERS
    int getBonusAttack() const
    {
        return this->bonusAttack;
    }
    void setBonusAttack(const int newBonusAtack)
    {
        this->bonusAttack = newBonusAtack;
    }
    int getBonusHealth() const
    {
        return this->bonusHealth;
    }
    void setBonusHealth(const int newBonusHealth)
    {
        this->bonusHealth = newBonusHealth;
    }
    void setName(const char *newName)
    {
        setterHelper(this->name, newName);
    }
    const char *getName()
    {
        return this->name;
    }
};

class Champion : public Item
{
private:
    int baseHealth;
    int baseAD;
    Item **inventory;
    int count;
    int capacity;
};