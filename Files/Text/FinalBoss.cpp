#include <iostream>
#include <cstring>
#include <fstream>

struct Hero
{
    char *name;
    int hp;
    int attack;

    void innit(const char *newName, const int newHp, const int newAttack)
    {
        name = new char[strlen(newName) + 1];
        strcpy(name, newName);
        hp = newHp;
        attack = newAttack;
    }

    void destroy()
    {
        delete[] name;
        name = nullptr;
    }

    // void write(ofstream& ) {

    // }
};

struct Roster
{
    Hero *hero;
    size_t count = 0;
    size_t cap = 2;

    void destroy()
    {
        // frees all of the heroes
        for (size_t i = 0; i < count; i++)
        {
            hero[i].destroy();
        }
        delete[] hero;
    }

    void resize()
    {
        cap *= 2;
        Hero *tmp = new Hero[cap];
        for (size_t i = 0; i < count; i++)
        {
            tmp[i].innit(hero[i].name, hero[i].hp, hero[i].attack);
        }
        Roster::destroy();
        hero = new Hero[cap];
        hero = tmp;
    }

    void innit(const char *newName, const int newHp, const int newAttack)
    {
        hero[count].innit(newName, newHp, newAttack);
        count++;
    }

    void addHero(const Hero *newHero)
    {
        if (count >= cap)
            resize();
    }
};
