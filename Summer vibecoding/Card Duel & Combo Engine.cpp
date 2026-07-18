#include <iostream>
#include <cstring>
#include <utility>
#include <exception>

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
        break;
    case SpellType::Continuous:
        std::cout << "Continuous Spell" << std::endl;
        break;
    case SpellType::Equip:
        std::cout << "Equip Spell" << std::endl;
        break;
    case SpellType::Field:
        std::cout << "Field Spell" << std::endl;
        break;
    case SpellType::Ritual:
        std::cout << "Ritual Spell" << std::endl;
        break;
    case SpellType::POLYMERIZATION:
        std::cout << "POLYMERIZATION" << std::endl;
        break;
    default:
        std::cout << "Normal Spell" << std::endl;
        break;
    }
    std::cout << std::endl;
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
        break;
    case TrapType::Continuous:
        std::cout << "Continuous Trap" << std::endl;
        break;
    default:
        std::cout << "Normal trap" << std::endl;
        break;
    }
    std::cout << std::endl;
}
class Card
{
private:
    char *name;
    char *effect;

    // Helper to deallocate dynamic memory safely
    void free()
    {
        delete[] name;
        delete[] effect;
    }

protected:
    // Helper for Rule of 3/5
    void swap(Card &other)
    {
        std::swap(this->name, other.name);
        std::swap(this->effect, other.effect);
    }

    // Helper to handle dynamic string assignment (deep copy)
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
    // Operator = using copy and swap idiom
    Card &operator=(Card other)
    {
        this->swap(other);
        return *this;
    }
    // Virtual destructor ensuring proper cleanup of derived classes
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

    // Virtual functions for polymorphic behavior
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

class DuelBoard
{
private:
    MonsterCard *monsterZones[5];
    Card *backrowZones[5]; // CAN BE SPELL OR TRAP OR PEN
    Card **Graveyard;
    int count;
    int capacity = 2;

    // Release all dynamically allocated resources
    void free()
    {
        for (size_t i = 0; i < 5; i++)
        {
            delete monsterZones[i];
            delete backrowZones[i];
            monsterZones[i] = nullptr;
            backrowZones[i] = nullptr;
        }
        for (size_t i = 0; i < count; i++)
        {
            delete Graveyard[i];
            Graveyard[i] = nullptr;
        }
        delete[] Graveyard;
        Graveyard = nullptr;
        count = 0;
        capacity = 0;
    }

    // Expand graveyard capacity when full
    void resize()
    {
        capacity *= 2;
        Card **newGraveyard = new Card *[capacity];
        for (size_t i = 0; i < count; i++)
        {
            newGraveyard[i] = this->Graveyard[i];
        }
        delete[] Graveyard;
        Graveyard = newGraveyard;
    }

    // Deep copy helper for Copy Constructor and Assignment Operator
    void copyFrom(const DuelBoard &other)
    {
        this->capacity = other.capacity;
        this->count = other.count;
        this->Graveyard = new Card *[capacity]; // Allocate new memory for graveyard
        for (size_t i = 0; i < count; i++)
        {
            this->Graveyard[i] = other.Graveyard[i]->clone(); // Deep copy using polymorphic clone
        }
        for (size_t i = 0; i < 5; i++)
        {
            this->monsterZones[i] = other.monsterZones[i] ? other.monsterZones[i]->clone() : nullptr;
            this->backrowZones[i] = other.backrowZones[i] ? other.backrowZones[i]->clone() : nullptr;
        }
    }

public:
    // Defaut Constructor
    DuelBoard() : count(0), capacity(2)
    {
        Graveyard = new Card *[capacity]; // Initial graveyard allocation
        for (size_t i = 0; i < 5; i++)
        {
            this->monsterZones[i] = nullptr;
            this->backrowZones[i] = nullptr;
        }
    }
    // Par Constructor
    DuelBoard(const int otherCount, const int otherCapacity, const MonsterCard *otherMonsterZones[5], const Card *otherBackrowZones[5], const Card **otherGraveyard) : count(otherCount), capacity(otherCapacity)
    {
        // Allocate and clone to ensure deep copy
        for (size_t i = 0; i < 5; i++)
        {
            this->monsterZones[i] = otherMonsterZones[i] ? otherMonsterZones[i]->clone() : nullptr;
            this->backrowZones[i] = otherBackrowZones[i] ? otherBackrowZones[i]->clone() : nullptr;
        }
        this->Graveyard = new Card *[capacity];
        for (size_t i = 0; i < count; i++)
        {
            this->Graveyard[i] = otherGraveyard[i] ? otherGraveyard[i]->clone() : nullptr;
        }
    }
    // Copy Constructor
    DuelBoard(const DuelBoard &other)
    {
        copyFrom(other);
    }
    // Operator =
    DuelBoard &operator=(const DuelBoard &other)
    {
        if (this != &other)
        {
            free(); // Clean up current state before copying
            copyFrom(other);
        }
        return *this;
    }
    ~DuelBoard()
    {
        free();
    }

    // THE FINAL BOSS!!!!!
    void playCard(Card *card)
    {
        // CASE 1 ~~> PENDULUM CARD
        // Checked first because Pendulum inherits from both Monster and Spell
        if (PendulumCard *pen = dynamic_cast<PendulumCard *>(card))
        {
            std::cout << "How would you like to play your pendulum card - as a pen scale or a monster?   "
                      << "enter 'm' for monster, 's' for scale." << std::endl;
            char penUse;
            std::cin >> penUse;
            if (penUse == 'm')
            {
                for (size_t i = 0; i < 5; i++)
                {
                    if (monsterZones[i] == nullptr)
                    {
                        monsterZones[i] = pen->clone();
                        return;
                    }
                }
            }
            if (penUse == 's')
            {
                if (backrowZones[0] == nullptr)
                {
                    backrowZones[0] = pen->clone();
                    return;
                }
                else if (backrowZones[4] == nullptr)
                {
                    backrowZones[4] = pen->clone();
                    return;
                }
            }
            return;
        }

        // CASE 2 ~~> MONSTER CARD
        // Cast to check if it's a monster and perform deep copy into zone
        if (MonsterCard *mon = dynamic_cast<MonsterCard *>(card))
        {
            for (size_t i = 0; i < 5; i++)
            {
                if (monsterZones[i] == nullptr)
                {
                    monsterZones[i] = mon->clone();
                    return;
                }
            }
            throw std::invalid_argument("Cannot play monster card because there are no unoccupied zones");
        }

        // CASE 3~~> SPELL CARD
        if (SpellCard *spell = dynamic_cast<SpellCard *>(card))
        {
            if (spell->getType() == SpellType::Continuous || spell->getType() == SpellType::Equip)
            {
                for (size_t i = 0; i < 5; i++)
                {
                    if (backrowZones[i] == nullptr)
                    {
                        backrowZones[i] = spell->clone();
                        return;
                    }
                }
                throw std::invalid_argument("Cannot play spell card because there are no unoccupied zones");
            }
            else
            {
                // Normal Spell -> Graveyard (needs automatic cleanup)
                if (count >= capacity)
                    resize();
                Graveyard[count++] = spell->clone();
                return;
            }
        }

        // CASE 4 ~~> TRAP CARD
        if (TrapCard *trap = dynamic_cast<TrapCard *>(card))
        {
            if (trap->getType() == TrapType::Continuous)
            {
                for (size_t i = 0; i < 5; i++)
                {
                    if (backrowZones[i] == nullptr)
                    {
                        backrowZones[i] = trap->clone();
                        return;
                    }
                }
                throw std::invalid_argument("Cannot play trap card because there are no unoccupied zones");
            }
            else
            {
                // Normal Trap -> Graveyard
                if (count >= capacity)
                    resize();
                Graveyard[count++] = trap->clone();
                return;
            }
        }
    }
};