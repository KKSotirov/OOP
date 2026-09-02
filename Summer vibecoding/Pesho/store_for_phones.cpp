#include <iostream>
#include <cstring>

class Phone
{
private:
    char *model;
    char *brand;
    double price;

    bool isValidPrice(const double _price)
    {
        return _price > 0;
    }

    void free()
    {
        delete[] model;
        model = nullptr;
        delete[] brand;
        brand = nullptr;
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

    void setAll(const char *_model, const char *_brand, const double _price)
    {
        setModel(_model);
        setBrand(_brand);
        setPrice(_price);
    }

public:
    // Default
    Phone() : model(nullptr), brand(nullptr), price(0) {}

    // Par constr
    Phone(const char *_model, const char *_brand, const double _price) : model(nullptr), brand(nullptr), price(0)
    {
        // we initialize object even when price is not valid ~> but we know cuz it is 0 leva
        setAll(_model, _brand, _price);
    }

    // Copy constructor
    Phone(const Phone &other) : model(nullptr), brand(nullptr)
    {
        setAll(other.model, other.brand, other.price);
    }

    // Operator =
    Phone &operator=(const Phone &other)
    {
        if (this != &other)
        {
            setAll(other.model, other.brand, other.price);
        }
        return *this;
    }

    virtual ~Phone()
    {
        free();
    }

    // Getters + Setters
    void setModel(const char *_model)
    {
        setterHelper(model, _model);
    }

    const char *getModel() const
    {
        return model;
    }

    void setBrand(const char *_brand)
    {
        setterHelper(brand, _brand);
    }

    const char *getBrand() const
    {
        return brand;
    }

    void setPrice(const double _price)
    {
        if (!isValidPrice(_price))
            return; // if invalid price -> we do not change original price!
        price = _price;
    }

    double getPrice() const
    {
        return price;
    }

    virtual Phone *clone() const
    {
        return new Phone(*this);
    }
};

class Smartphone : public Phone
{
private:
    char *os;
    unsigned storage;

    void calcNewPrice()
    {
        this->setPrice(this->getPrice() + storage);
    }

    void free()
    {
        delete[] os;
        os = nullptr;
    }

public:
    // RO3
    Smartphone() : Phone(), os(nullptr), storage(0) {}

    Smartphone(const char *_model, const char *_brand, const double _price, const char *_os, const unsigned _storage) : Phone(_model, _brand, _price), os(nullptr), storage(_storage)
    {
        setOs(_os);
        calcNewPrice();
    }

    Smartphone(const Smartphone &other) : Phone(other), os(nullptr), storage(other.storage)
    {
        setOs(other.os);
    }

    Smartphone &operator=(const Smartphone &other)
    {
        if (this != &other)
        {
            this->Phone::operator=(other);
            setOs(other.os);
            setStorage(other.storage);
        }
        return *this;
    }

    ~Smartphone()
    {
        free();
    }
    // Getters + Setters
    void setOs(const char *_os)
    {
        setterHelper(os, _os);
    }

    const char *getOs() const
    {
        return os;
    }

    void setStorage(const unsigned _storage)
    {
        storage = _storage;
    }

    unsigned getStorage() const
    {
        return storage;
    }

    Smartphone *clone() const override
    {
        return new Smartphone(*this);
    }
};

class Shop
{
    // BullFMI Vietnam flashbacks -> the bushes are speaking C++    0_0
private:
    Phone **phones;
    double budget;
    size_t size;
    size_t capacity = 2;

    void resize()
    {
        capacity *= 2;
        Phone **tmp = new Phone *[capacity];
        for (size_t i = 0; i < size; i++)
        {
            tmp[i] = phones[i];
        }
        delete[] phones;
        phones = tmp;
    }

    void free()
    {
        for (size_t i = 0; i < size; i++)
        {
            delete phones[i];
        }
        delete[] phones;
    }

    void copyFrom(const Shop &other)
    {
        budget = other.budget;
        capacity = other.capacity;
        size = other.size;
        phones = new Phone *[capacity];
        for (size_t i = 0; i < size; i++)
        {
            phones[i] = other.phones[i]->clone();
        }
    }

public:
    Shop(const double _budget) : budget(_budget), size(0)
    {
        phones = new Phone *[capacity];
    }

    Shop(const Shop &other) : budget(other.budget)
    {
        copyFrom(other);
    }

    Shop &operator=(const Shop &other)
    {
        if (this != &other)
        {
            free();
            copyFrom(other);
        }
        return *this;
    }

    ~Shop()
    {
        free();
    }

    double getBudget() const
    {
        return budget;
    }

    bool addPhone(const Phone &_phone)
    {
        if (budget > _phone.getPrice())
        {
            if (size >= capacity)
                resize();
            phones[size++] = _phone.clone();
            budget -= _phone.getPrice();
            return true;
        }
        return false;
    }

    bool sellPhone(const char *_model, const char *_brand)
    {
        // We sell the first Iphone 13 Pro we find
        int indexSold = -1;
        for (size_t i = 0; i < size; i++)
        {
            if (strcmp(phones[i]->getModel(), _model) == 0)
                if (strcmp(phones[i]->getBrand(), _brand) == 0)
                {
                    // we found our phone, lets clear it from our "inventory"
                    indexSold = i;
                    budget += phones[i]->getPrice();
                    delete phones[i];
                    break; // we get out of the for cycle, in order not to sell all our models of this phone
                }
        }
        if (indexSold == -1)
            return false; // We haven't got this phone in stock

        // Since our phone order is of no use to us, we are going to be lazy/optimal
        phones[indexSold] = phones[--size];
        return true;
    }

    bool removeCheapestSamsung()
    {
        int indexCheapestSamsung = -1;
        double cheapestSamsung = 0.0;
        bool foundSamsung = false;

        for (size_t i = 0; i < size; i++)
        {
            if (strcmp("Samsung", phones[i]->getBrand()) == 0)
            {
                if (!foundSamsung)
                {
                    indexCheapestSamsung = i;
                    cheapestSamsung = phones[i]->getPrice();
                    foundSamsung = true;
                }
                if (cheapestSamsung > phones[i]->getPrice())
                {
                    indexCheapestSamsung = i;
                    cheapestSamsung = phones[i]->getPrice();
                }
            }
        }
        if (!foundSamsung)
            return false;

        // We have found our samsung
        sellPhone(phones[indexCheapestSamsung]->getModel(), phones[indexCheapestSamsung]->getBrand());
        return true;
    }
};

int main()
{
    std::cout << "Hello there traveller! \n";

    // Lets first create a bunch of Phones
    Smartphone iphone13_1("13 Pro", "Iphone", 1299.9, "poslednata brat", 512);
    Smartphone iphone13_2("13 Pro", "Iphone", 1299.9, "poslednata brat", 512);
    Smartphone iphone12("12 Pro", "Iphone", 999.9, "predPoslednata brat", 512);
    Smartphone iphone1("1 OG", "Iphone", 30000.0, "purvata brat", 10000);

    Smartphone samsungS26_1("S 26", "Samsung", 999.9, "poslednata brat", 512);
    Smartphone samsungS26_2("S 26", "Samsung", 999.9, "poslednata brat", 512);
    Smartphone samsungS24("S 24", "Samsung", 499.9, "predPoslednata brat", 512);
    Smartphone samsungS1("S 1 OG", "Samsung", 20000.0, "purvata brat", 15000);

    // Lets now create our shop
    Shop chobanovPhones(100000.0);
    std::cout << "ChobanovPhones budget at start = " << chobanovPhones.getBudget() << "\n";

    // Lets purchase some phones
    chobanovPhones.addPhone(iphone13_1);
    chobanovPhones.addPhone(iphone13_2);
    chobanovPhones.addPhone(iphone12);
    chobanovPhones.addPhone(iphone1);
    chobanovPhones.addPhone(samsungS26_1);
    chobanovPhones.addPhone(samsungS26_2);
    chobanovPhones.addPhone(samsungS24);
    chobanovPhones.addPhone(samsungS1);

    std::cout << "ChobanovPhones budget after buying phones = " << chobanovPhones.getBudget() << "\n";
    // Removing the cheapest Samsung
    chobanovPhones.removeCheapestSamsung();

    // Lets sell some phones
    chobanovPhones.sellPhone("13 Pro", "Iphone");
    chobanovPhones.sellPhone("13 Pro", "Iphone");
    chobanovPhones.sellPhone("12 Pro", "Iphone");
    chobanovPhones.sellPhone("S 26", "Samsung");
    chobanovPhones.sellPhone("S 24", "Samsung");

    std::cout << "ChobanovPhones budget after buying and selling = " << chobanovPhones.getBudget() << "\n";

    return 0;
}