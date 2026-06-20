#include <iostream>
#include <utility>
#include <cstring>

class Item
{
private:
    char *name;
    double weight;
    static int totalItemsCreated;
    const int id;

    void copyName(const char *otherName)
    {
        if (otherName == nullptr)
        {
            this->name = nullptr;
        }
        else
        {
            this->name = new char[strlen(otherName) + 1];
            strcpy(this->name, otherName);
        }
    }

    void free()
    {
        delete[] this->name;
    }

protected:
    void printName() const
    {
        std::cout << name;
    }

    void swap(Item &other)
    {
        std::swap(this->name, other.name);
        std::swap(this->weight, other.weight);
    }

public:
    // def constructor
    Item() : weight(0), id(++totalItemsCreated), name(nullptr) {}
    // RO3
    // parameterized constructor
    Item(const char *otherName, const double otherWeight) : id(++totalItemsCreated), weight(otherWeight)
    {
        copyName(otherName);
    }
    // copy constructor
    Item(const Item &other) : id(++totalItemsCreated), weight(other.weight)
    {
        copyName(other.name);
    }
    // operator = using copy and swap Pesho method
    Item &operator=(Item other)
    {
        this->swap(other);
        return *this;
    }
    // Virtual Destructor
    virtual ~Item()
    {
        totalItemsCreated--;
        free();
    }

    // Virtual functions
    virtual Item *clone() const = 0;
    virtual void print() const = 0;

    const double getWeight() const
    {
        return weight;
    }
};

int Item::totalItemsCreated = 0;

class Weapon : virtual public Item
{
private:
    int attackPower;

public:
    // Par  Constructor
    Weapon(const int attack, const char *name, const double weight) : Item(name, weight), attackPower(attack) {}
    // Copy Constructor
    Weapon(const Weapon &other) : Item(other), attackPower(other.attackPower) {}

    // Overriding !!!
    Weapon *clone() const override
    {
        return new Weapon(*this);
    }
    void print() const override
    {
        std::cout << "[Item] ~> Weapon: name = ";
        printName();
        std::cout << ", weight = " << getWeight()
                  << ", and attack power = " << attackPower
                  << std::endl;
    }

    const int getAttack() const
    {
        return this->attackPower;
    }
};

class Armor : virtual public Item
{
private:
    int defensePower;
    char *material;

    void copyPar(const char *otherMaterial)
    {
        if (otherMaterial == nullptr)
        {
            this->material = nullptr;
            return;
        }
        this->material = new char[strlen(otherMaterial) + 1];
        strcpy(this->material, otherMaterial);
    }

    void swap(Armor &other)
    {
        std::swap(this->material, other.material);
        std::swap(this->defensePower, other.defensePower);
    }

    void free()
    {
        delete[] material;
    }

public:
    // DYN MEMORY ==> RO3
    //  Par constructor
    Armor(const int otherDefense, const char *otherMaterial, const char *name, const double weight) : Item(name, weight), defensePower(otherDefense)
    {
        copyPar(otherMaterial);
    }
    // Copy Constructor
    Armor(const Armor &other) : Item(other), defensePower(other.defensePower)
    {
        copyPar(other.material);
    }
    // Operator =  using Pesho's copy and swap
    Armor &operator=(Armor other)
    {
        this->Item::swap(other);
        this->swap(other);
        return *this;
    }
    // Destructor
    ~Armor()
    {
        free();
    }

    // OVERRIDING !!
    Armor *clone() const override
    {
        return new Armor(*this);
    }
    void print() const override
    {
        std::cout << "[Item] ~> Armor: name = ";
        printName();
        std::cout << ", weight = " << getWeight() << ", material = "
                  << material << ", and defense power = "
                  << defensePower << std::endl;
    }

    const int getDeff() const
    {
        return this->defensePower;
    }
    const char *getMaterial() const
    {
        return this->material;
    }
};

class MagicalArtifact : public Weapon, public Armor
{
private:
    int manaBonus;

public:
    // NO DYN MEMORY ==> NO RO3
    // Parametirized constructor
    MagicalArtifact(const int otherManaBonus, const int otherDefense, const char *otherMaterial, const int attack, const char *name,
                    const double weight) : Item(name, weight), Weapon(attack, name, weight), Armor(otherDefense, otherMaterial, name, weight), manaBonus(otherManaBonus) {}

    MagicalArtifact *clone() const override
    {
        return new MagicalArtifact(*this);
    }
    void print() const override
    {
        std::cout << "[Item] ~> {*^ Magical Artifact ^*}: name = ";
        printName();
        std::cout << ", weight = " << getWeight() << ", material = "
                  << getMaterial() << ", defense power   = "
                  << getDeff() << ", attack  power   = "
                  << getAttack() << " and ^Mana bonus^ = "
                  << this->manaBonus << std::endl;
    }
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ PART 2
class Inventory
{
private:
    Item **items;
    int count;
    int capacity;

    void free()
    {
        for (size_t i = 0; i < count; i++)
        {
            delete this->items[i];
        }
        delete[] this->items;
    }

    void swap(Inventory &other)
    {
        std::swap(this->items, other.items);
        std::swap(this->capacity, other.capacity);
        std::swap(this->count, other.count);
    }

    void copyObj(const Inventory &other)
    {
        this->count = other.count;
        this->capacity = other.capacity;
        this->items = new Item *[other.capacity];
        for (size_t i = 0; i < this->count; i++)
        {
            this->items[i] = other.items[i]->clone();
        }
    }

    void resize()
    {
        capacity *= 2;
        Item **tmpItems = new Item *[capacity];
        for (size_t i = 0; i < this->count; i++)
        {
            tmpItems[i] = this->items[i];
        }
        delete[] this->items;
        this->items = tmpItems;
    }

    void tighten(const int index)
    {
        for (size_t i = index; i < (count - 1); i++)
        {
            this->items[i] = this->items[i + 1];
        }
        this->items[count - 1] = nullptr;
        count--;
    }

public:
    // DYNAMIC MEMORY ==> RO3
    // Default constructor
    Inventory() : capacity(2), count(0)
    {
        this->items = new Item *[capacity];
    }
    // Parametirized constructor
    // ~~~~~~~~~~~
    // Copy constructor
    Inventory(const Inventory &other)
    {
        copyObj(other);
    }
    Inventory &operator=(Inventory other)
    {
        this->swap(other);
        return *this;
    }
    ~Inventory()
    {
        free();
    }

    void addItem(Item *addedItem)
    {
        if (addedItem == nullptr)
            return;
        if (count >= capacity)
            resize();
        items[count] = addedItem;
        count++;
    }

    void removeItem(const int index)
    {
        if (index >= count || index < 0)
            return;
        delete this->items[index];
        tighten(index);
    }
};

int main()
{
    return 0;
}