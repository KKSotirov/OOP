#include <iostream>
#include <utility>
#include <cstring>

class Device
{
private:
    const size_t id;
    static size_t count;
    char *brand;

    void free()
    {
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

public:
    // Default constructor
    Device() : brand(nullptr), id(++count) {}

    // Par constructor
    Device(const char *newBrand) : brand(nullptr), id(++count)
    {
        setterHelper(brand, newBrand);
    }

    // Copy constructor
    Device(const Device &other) : brand(nullptr), id(++count)
    {
        setterHelper(brand, other.brand);
    }

    // Operator =
    Device &operator=(const Device &other)
    {
        if (this != &other)
        {
            setterHelper(this->brand, other.brand);
        }
        return *this;
    }

    // Virtual destructor
    virtual ~Device()
    {
        free();
    }

    // Getters + Setters
    size_t getId() const
    {
        return this->id;
    }

    const char *getBrand() const
    {
        return this->brand;
    }

    void setBrand(const char *newBrand)
    {
        setterHelper(this->brand, newBrand);
    }

    // Virtual functions
    virtual Device *clone() const = 0;
    virtual void print() const = 0;
};

size_t Device::count = 0;

class Laptop : public Device
{
private:
    double cpuFreq;

public:
    // No dyn mem ~~> no RO3

    // Def constructor
    Laptop() : cpuFreq(0.0), Device() {}

    // Par constructor
    Laptop(const double newCpuFreq, const char *newBrand) : cpuFreq(newCpuFreq), Device(newBrand) {}

    // Copy constructor -> just to be sure
    Laptop(const Laptop &other) : cpuFreq(other.cpuFreq), Device(other) {}

    // Getters + Setters
    double getCpuFreq() const
    {
        return cpuFreq;
    }
    void setCpuFreq(const double newCpuFreq)
    {
        cpuFreq = newCpuFreq;
    }

    // Virtual functions
    Laptop *clone() const override
    {
        return new Laptop(*this);
    }

    void print() const override
    {
        std::cout << "Brand: " << this->getBrand()
                  << ", cpuFreq = " << cpuFreq << "\n";
    }
};

class Phone : public Device
{
private:
    int batteryCapacity;

public:
    // No dyn mem -> No RO3

    // Default constructor
    Phone() : batteryCapacity(0), Device() {}

    // Par constructor
    Phone(const int newBatteryCapacity, const char *newBrand) : batteryCapacity(newBatteryCapacity), Device(newBrand) {}

    // Copy constructor -> just to be sure
    Phone(const Phone &other) : batteryCapacity(other.batteryCapacity), Device(other) {}

    // Getters + Setters
    int getBatteryCapacity() const
    {
        return batteryCapacity;
    }

    void setBatteryCapacity(const int newBatteryCapacity)
    {
        batteryCapacity = newBatteryCapacity;
    }

    // Virtual functions
    Phone *clone() const override
    {
        return new Phone(*this);
    }
    void print() const override
    {
        std::cout << "Brand: " << this->getBrand()
                  << ", batteryCapacity = " << batteryCapacity << "\n";
    }
};