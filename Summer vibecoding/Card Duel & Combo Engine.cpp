#include <iostream>
#include <cstring>
#include <utility>

enum class SpellType
{
    QuickPlay,
    Continuous,
    Equip,
    Field,
    Ritual,
    POLYMERIZATION,
    Normal
};
void printSpellType(SpellType spellType)
{
    switch (spellType)
    {
    case SpellType::QuickPlay:
        std::cout << "QuickPlay Spell" << std::endl;
    case SpellType::Continuous:
        std::cout << "Continuous Spell" << std::endl;
    case SpellType::Equip:
        std::cout << "Equip Spell" << std::endl;
    case SpellType::Field:
        std::cout << "Field Spell" << std::endl;
    case SpellType::Ritual:
        std::cout << "Ritual Spell" << std::endl;
    case SpellType::POLYMERIZATION:
        std::cout << "POLYMERIZATION" << std::endl;
    default:
        std::cout << "Normal Spell" << std::endl;
    }
}
enum class TrapType
{
    Counter,
    Continuous,
    Normal
};

void printTrapType(TrapType trapType)
{
    switch (trapType)
    {
    case TrapType::Counter:
        std::cout << "Counter Trap" << std::endl;
    case TrapType::Continuous:
        std::cout << "Continuous Trap" << std::endl;
    default:
        std::cout << "Normal trap" << std::endl;
    }
}
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
    Card(const Card &other) : name(nullptr), effect(nullptr)
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
    void setDefense(const int newdefense)
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

class SpellCard : virtual public Card
{
private:
    SpellType type;

public:
    // Def constructor
    SpellCard() : Card(), type(SpellType::Normal) {}
    // Par constructor
    SpellCard(const SpellType newType, const char *cardName, const char *cardEffect) : Card(cardName, cardEffect), type(newType) {}

    // Getters + Setters
    void setType(const SpellType newType)
    {
        this->type = newType;
    }
    SpellType getType() const
    {
        return this->type;
    }

    // Virtual functions
    virtual void play() const override
    {
        std::cout << "[SPELL CARD] ~~> {NAME = " << this->getName()
                  << "}, {EFFECT = " << this->getEffect() << "} , {Spell Type -> ";
        printSpellType(this->type);
        std::cout << "} !!" << std::endl;
    }
    virtual SpellCard *clone() const override
    {
        return new SpellCard(*this);
    }
};

class TrapCard : virtual public Card
{
private:
    TrapType type;

public:
    // Def Constructor
    TrapCard() : Card(), type(TrapType::Normal) {}
    // Par Constructor
    TrapCard(const TrapType cardType, const char *cardName, const char *cardEffect) : Card(cardName, cardEffect), type(cardType) {}

    // Getters + Setters
    void setType(const TrapType newType)
    {
        this->type = newType;
    }
    TrapType getType() const
    {
        return this->type;
    }

    // Virtual functions
    virtual void play() const override
    {
        std::cout << "[TRAP CARD] ~~> {NAME = " << this->getName()
                  << "}, {EFFECT = " << this->getEffect() << "} {Trap type -> ";
        printTrapType(this->type);
        std::cout << "} !!" << std::endl;
    }
    virtual TrapCard *clone() const
    {
        return new TrapCard(*this);
    }
};

class PendulumCard : public SpellCard, public MonsterCard
{
private:
    int pendulumScale;

    bool isValidPenScale(const int scale)
    {
        return (scale > 0 && scale < 13);
    }

public:
    // Def constructor
    PendulumCard() : Card(), MonsterCard(), SpellCard(), pendulumScale(1) {}
    // Par constructor
    PendulumCard(const int penScale, const int penAttack, const int penDeff, const char *penName, const char *penEffect) : Card(penName, penEffect), MonsterCard(penAttack, penDeff, penName, penEffect), SpellCard(SpellType::Continuous, penName, penEffect)
    {
        if (isValidPenScale(penScale))
            this->pendulumScale = penScale;
        // else ~PendulumCard();
    }
    // Copy constructor -> no need because of no dyn memory
    // Operator = -> no need because of no dyn memory
    // Destructor -> no need becase of no dyn memory
    virtual PendulumCard *clone() const override
    {
        return new PendulumCard(*this);
    }
    virtual void play() const override
    {
        std::cout << "[PENDULUM CARD] ~~> {NAME = " << this->getName()
                  << "}, {EFFECT = " << this->getEffect() << "}, "
                  << "{ATTACK = " << this->getAttack() << ", DEFFENSE = " << this->getDefense()
                  << "}!!" << std::endl;
    }
};
