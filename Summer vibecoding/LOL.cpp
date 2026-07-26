#include <iostream>
#include <cstring>
#include <stdexcept>
#include <utility>

class Entity
{
private:
    char *name;
    const int id;
    static int counter;

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

    void swap(Entity &other) noexcept
    {
        std::swap(this->name, other.name);
        // id's cannot be copied
    }

public:
    // Dyn Mem ==> RO3
    // Def constr
    Entity() : name(nullptr), id(++counter) {}
    // Par constr
    Entity(const char *newName) : name(nullptr), id(++counter)
    {
        setterHelper(this->name, newName);
    }
    // Copy constructor
    Entity(const Entity &other) : id(++counter), name(nullptr)
    {
        setterHelper(this->name, other.name);
    }
    // Operator =
    Entity &operator=(Entity other)
    {
        this->swap(other);
        return *this;
    }
    // Destructor
    ~Entity()
    {
        free();
    }

    // Operator >
    bool operator>(const Entity &other) const
    {
        return this->id > other.id;
    }
    bool operator<(const Entity &other) const
    {
        return other.id > this->id;
    }
    friend std::ostream &operator<<(std::ostream &os, const Entity &object)
    {
        std::cout << "[KEY INFO] ~> {ID = " << object.id
                  << "},  {NAME = " << object.name << " }"
                  << std::endl;
    }
    virtual Entity *operator+(const Entity *other) = 0;
    virtual Entity *operator*(const int multiplier) const = 0;
    virtual Entity *clone() const = 0;
    virtual Entity *activate() const = 0; // use ability
    virtual Entity &operator++() = 0;     // Level up
};

int Entity::counter = 0;
