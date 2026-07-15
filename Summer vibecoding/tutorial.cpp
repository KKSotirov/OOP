#include <iostream>
#include <utility>
#include <cstring>

class Item
{
private:
    char *name;
    int bonusHealth;
    int bonusAttack;

protected:
    void copyObj(const char *str)
    {
    }

public:
    // default constructor
    Item() : name(nullptr), bonusHealth(0), bonusAttack(0) {}
    // parametrised constructor
    Item(const char *str, const int bHealth, const int bAttack) : bonusHealth(bHealth), bonusAttack(bAttack)
    {
    }
};