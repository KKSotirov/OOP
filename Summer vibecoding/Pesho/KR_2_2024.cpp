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
