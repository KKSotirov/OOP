#include <iostream>
#include <cstring>
#include <utility>

class Device
{
private:
    char *brand;
    int power;

    void swap(Device &other) noexcept
    {
        std::swap(this->brand, other.brand);
        std::swap(this->power, other.power);
    }

    void free()
    {
        delete[] this->brand;
        brand = nullptr;
        power = 0;
    }

    void copy(const Device &other)
    {
        if (other.brand == nullptr)
        {
            this->brand = nullptr;
            this->power = other.power;
            return;
        }
        this->brand = new char[strlen(other.brand) + 1];
        strcpy(this->brand, other.brand);
        this->power = other.power;
    }

public:
    Device(const char *nBrand, int nPower)
    {
        char *tmpBr = nullptr;
        if (nBrand == nullptr || nPower < 0)
            throw std::invalid_argument("Invalid args in par. constructor for Device!");

        tmpBr = new char[strlen(nBrand) + 1];
        strcpy(tmpBr, nBrand);
        this->brand = tmpBr;
        this->power = nPower;
    }

    // R03
    Device(const Device &other)
    {
        copy(other);
    }

    Device &operator=(Device other)
    {
        this->swap(other);
        return *this;
    }

    virtual ~Device()
    {
        free();
    }

    virtual void start() const = 0;

    const char *getBrand() const
    {
        return brand;
    }
};

class Laptop : public Device
{
private:
    int battery_life;

    void printBrand(const Laptop &obj) const
    {
        char *tmpArr = new char[strlen(obj.getBrand()) + 1];
        char *tmpStart = tmpArr;
        strcpy(tmpArr, obj.getBrand());
        while (*tmpArr)
        {
            std::cout << *tmpArr;
            tmpArr++;
        }
        delete[] tmpStart;
    }

public:
    // parameterized constructor
    // predef virtual function
    Laptop(const char *brand, const int power, const int battery) : Device(brand, power), battery_life(battery) {}

    void start() const override
    {
        std::cout << "Brand: ";
        printBrand(*this);
        std::cout << ", Booting IOS ..." << std::endl;
    }
};