#include <iostream>
#include <cstring>
#include <utility>

class Card
{
private:
    char *name;
    char *effect;

    void free()
    {
        delete[] name;
        delete[] effect;
    }

protected:
    void swap(Card &other)
    {
        std::swap(this->name, other.name);
        std::swap(this->effect, other.effect);
    }

    void setterHelper(char *&dest, const char *src)
    {
        char *temp = src ? new char[strlen(src) + 1] : nullptr;
        if (src != nullptr)
            strcpy(temp, src);
        delete[] dest;
        dest = temp;
    }

public:
    // Def constructor
    Card() : name(nullptr), effect(nullptr) {}
    // Par constructor
    Card(const char *newName, const char *newEffect) : name(nullptr), effect(nullptr)
    {
        setterHelper(this->name, newName);
        setterHelper(this->effect, newEffect);
    }
    // Copy constructor
    Card(const Card &other)
    {
        setterHelper(this->name, other.getName());
        setterHelper(this->effect, other.getEffect());
    }
    // Operator = using copy and swap
    Card &operator=(Card other)
    {
        this->swap(other);
        return *this;
    }
    // Virtual destructor
    virtual ~Card()
    {
        free();
    }

    // GETTERS + SETTERS
    void setName(const char *newName)
    {
        setterHelper(this->name, newName);
    }
    char *getName() const
    {
        return this->name;
    }
    void setEffect(const char *newEffect)
    {
        setterHelper(this->effect, newEffect);
    }
    char *getEffect() const
    {
        return this->effect;
    }

    // Virtual functions
    virtual void play() const = 0;
    virtual Card *clone() const = 0;
};

class MonsterCard : virtual public Card
{
private:
    int attack;
    int defense;

public:
    // Default constructor
    MonsterCard() : Card(), attack(0), defense(0) {}
    // Par constructor
    MonsterCard(const int newAttack, const int newDefense, const char *cardName, const char *cardEffect) : Card(cardName, cardEffect), attack(newAttack), defense(newDefense) {}
    // NO dyn mem ~~> no ro3
    int getAttack() const
    {
        return this->attack;
    }
    void setAttack(const int newAttack)
    {
        this->attack = newAttack;
    }
    int getDefense() const
    {
        return this->defense;
    }
    void setAefense(const int newdefense)
    {
        this->defense = newdefense;
    }

    // Virtual functions
    virtual MonsterCard *clone() const override
    {
        return new MonsterCard(*this);
    }
    virtual void play() const override
    {
        std::cout << "[Card Name] ~~> " << this->getName()
                  << ", with stats : {ATK = " << this->attack
                  << "}, {DEFF = " << this->defense << "} !" << std::endl;
    }
};
