#include <iostream>
#include <cstring>
#include <cmath>

enum class WeaponType
{
    Sword,
    Wand,
    Staff,
    Axe
};

void printWeaponType(WeaponType _weapon)
{
    switch (_weapon)
    {
    case WeaponType::Sword:
        std::cout << "Wielding a Sword! \n";
        break;
    case WeaponType::Wand:
        std::cout << "Wielding a Wand! \n";
        break;
    case WeaponType::Staff:
        std::cout << "Wielding a Staff! \n";
        break;
    default:
        std::cout << "Wielding an Axe! \n";
        break;
    }
    std::cout << "\n";
}

struct Position
{
    int x;
    int y;

    bool isAdjacentTo(const Position &other) const
    {
        int dx = abs(x - other.x);
        int dy = abs(y - other.y);
        return (dx == 1) && (dy == 1);
    }
};

class Player
{
private:
    char *name;
    WeaponType weapon;
    Position pos;
    unsigned ad;
    unsigned hp;

    void free()
    {
        delete[] name;
        name = nullptr;
    }

protected:
    void setterHelper(char *&dest, const char *src)
    {
        char *tmp = src ? new char[strlen(src) + 1] : nullptr;
        if (src)
            strcpy(tmp, src);
        delete[] dest;
        dest = tmp;
    }

public:
    // RO3
    Player(const char *_name, const WeaponType _weapon, const Position _pos, const unsigned _ad, const unsigned _hp) : name(nullptr), weapon(_weapon), pos(_pos), ad(_ad), hp(_hp)
    {
        setName(_name);
    }

    Player(const Player &other) : name(nullptr), weapon(other.weapon), pos(other.pos), ad(other.ad), hp(other.hp)
    {
        setName(other.name);
    }

    // Player operator=(const Player &other)
    // {
    //     if (this != &other)
    //     {
    //         setName(other.name);
    //     }
    //     return *this;
    // }

    virtual ~Player()
    {
        free();
    }

    // GETTERS + SETTERS

    const char *getName() const
    {
        return name;
    }

    void setName(const char *_name)
    {
        setterHelper(name, _name);
    }

    unsigned getHp() const
    {
        return hp;
    }

    void setHp(const unsigned _hp)
    {
        hp = _hp;
    }

    void setAd(const unsigned _ad)
    {
        ad = _ad;
    }

    WeaponType getWeaponType() const
    {
        return weapon;
    }

    void setWeaponType(const WeaponType _weapon)
    {
        weapon = _weapon;
    }

    // Virtual functions
    virtual Player *clone() const = 0;
    virtual void printInfo() const
    {
        std::cout << "Player name: " << name
                  << ", with hp = " << hp
                  << ", attack damage = " << ad
                  << "position on map (" << pos.x
                  << "," << pos.y << ") and ";
        printWeaponType(weapon);
        std::cout << "\n";
    }

    virtual unsigned getAd() const
    {
        return ad;
    }

    // move, handle attack, attack, get attack power, print
    virtual bool move(const int targetX, const int targetY, const int moves)
    {
        if (pos.x == targetX && pos.y == targetY)
        {
            std::cout << "Successfully reached position : [" << targetX << "," << targetY << "]  after " << moves << " moves. \n";
            return true;
        }

        if (pos.x < targetX)
            pos.x++;
        else if (pos.x > targetX)
            pos.x--;

        if (pos.y < targetY)
            pos.y++;
        else if (pos.y > targetY)
            pos.y--;

        return move(targetX, targetY, moves + 1);
    }

    bool isDead() const
    {
        return hp == 0;
    }

    virtual void tankAttack(const unsigned dmg)
    {
        if (dmg >= hp)
            hp = 0;
        else
            hp -= dmg;
    }

    virtual void dealDmg(Player *target)
    {
        target->tankAttack(this->getAd());
    }
};

class Warrior : public Player
{
private:
    unsigned armor;

public:
    // No RO3
    Warrior(const char *_name, const WeaponType _weapon, const Position _pos, const unsigned _ad, const unsigned _hp, const unsigned _armor) : Player(_name, _weapon, _pos, _ad, _hp), armor(_armor) {}

    Warrior(const Warrior &other) : Player(other), armor(other.armor) {}

    // Getters + Setters
    unsigned getArmor() const
    {
        return armor;
    }

    void setArmor(const unsigned _armor)
    {
        armor = _armor;
    }

    // Virtual functions
    Warrior *clone() const override
    {
        return new Warrior(*this);
    }

    void printInfo() const override
    {
        this->Player::printInfo();
        std::cout << "Warrior type with " << armor << " armor! \n";
    }

    void tankAttack(unsigned dmg) override
    {
        if (armor > dmg)
            armor -= dmg;
        else
        {
            armor = 0;
            dmg -= armor;
            this->Player::tankAttack(dmg);
        }
    }
};

class Mage : public Player
{
private:
    char *spell;
    double poison;

    void free()
    {
        delete[] spell;
        spell = nullptr;
    }

    bool isValidPoison(const double _poison) const
    {
        return (_poison > 0) && (_poison < 1);
    }

public:
    // RO3
    Mage(const char *_name, const WeaponType _weapon, const Position _pos, const unsigned _ad, const unsigned _hp, const char *_spell, const double _poison) : Player(_name, _weapon, _pos, _ad, _hp), spell(nullptr)
    {
        poison = isValidPoison(_poison) ? _poison : 0.0;
        setSpell(_spell);
    }

    Mage(const Mage &other) : Player(other), spell(nullptr), poison(other.poison)
    {
        setSpell(other.spell);
    }

    Mage &operator=(const Mage &other)
    {
        if (this != &other)
        {
            this->Player::operator=(other);
            poison = other.getPoison();
            setSpell(other.getSpell());
        }
        return *this;
    }

    ~Mage() override
    {
        free();
    }

    // Getters + Setters
    const char *getSpell() const
    {
        return spell;
    }

    void setSpell(const char *_spell)
    {
        setterHelper(spell, _spell);
    }

    double getPoison() const
    {
        return poison;
    }

    void setPoison(const double _poison)
    {
        poison = _poison;
    }

    // Virtual functions
    Mage *clone() const override
    {
        return new Mage(*this);
    }

    void printInfo() const override
    {
        this->Player::printInfo();
        std::cout << "Mage with unique spell  = " << spell << " and posion coefficient = " << poison << "\n";
    }

    void dealDmg(Player *other) override
    {
        other->setHp(other->getHp() * poison);
        this->Player::dealDmg(other);
    }
};

class Necromancer : public Mage
{
private:
    Player **possessedCorpses;
    size_t defeated_count;
    size_t defeated_capacity;

    void freeNecromancer()
    {
        for (size_t i = 0; i < defeated_count; i++)
        {
            delete possessedCorpses[i];
        }
        delete[] possessedCorpses;
        possessedCorpses = nullptr;
    }

    void resizePosessed()
    {
        defeated_capacity *= 2;
        Player **tmp = new Player *[defeated_capacity];
        for (size_t i = 0; i < defeated_count; i++)
        {
            tmp[i] = possessedCorpses[i];
        }
        delete[] possessedCorpses;
        possessedCorpses = tmp;
    }

    void copyNecro(const Necromancer &other)
    {
        defeated_count = other.defeated_count;
        defeated_capacity = other.defeated_capacity;
        Player **possessedCorpses = new Player *[defeated_capacity];
        for (size_t i = 0; i < defeated_count; i++)
        {
            possessedCorpses[i] = other.possessedCorpses[i]->clone();
        }
    }

public:
    Necromancer(const char *_name, const WeaponType _weapon, const Position _pos, const unsigned _ad, const unsigned _hp, const char *_spell, const double _poison) : Mage(_name, _weapon, _pos, _ad, _hp, _spell, _poison), defeated_count(0), defeated_capacity(2)
    {
        possessedCorpses = new Player *[defeated_capacity];
    }

    Necromancer(const Necromancer &other) : Mage(other), possessedCorpses(nullptr)
    {
        copyNecro(other);
    }

    Necromancer &operator=(const Necromancer &other)
    {
        if (this != &other)
        {
            copyNecro(other);
            this->Mage::operator=(other);
        }
        return *this;
    }

    ~Necromancer() override
    {
        freeNecromancer();
    }

    // Virtual functions
    Necromancer *clone() const override
    {
        return new Necromancer(*this);
    }

    void printInfo() const override
    {
        this->Mage::printInfo();
        std::cout << "This is no mere mage ~> This is a 𝓝𝓮𝓬𝓻𝓸𝓶𝓪𝓷𝓬𝓮𝓻 ! \n";
    }

    void dealDmg(Player *other) override
    {
        this->Mage::dealDmg(other);
        if (defeated_count > 0)
        {
            for (size_t i = 0; i < defeated_count; i++)
            {
                other->tankAttack(possessedCorpses[i]->getAd());
            }
        }

        if (other->isDead())
        {
            // We possess him!
            if (defeated_count >= defeated_capacity)
                resizePosessed();

            possessedCorpses[defeated_count++] = other->clone();
        }
    }
};