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

enum class TrapType
{
    Counter,
    Continuous,
    Normal
};

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
                  << "}, {EFFECT = " << this->getEffect() << "} !!" << std::endl;
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
                  << "}, {EFFECT = " << this->getEffect() << "} !!" << std::endl;
    }
    virtual TrapCard *clone() const
    {
        return new TrapCard(*this);
    }
};