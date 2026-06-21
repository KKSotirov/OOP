#include <iostream>
#include <cstring>
#include <utility>
#include <stdexcept>

enum class Type
{
    mobile,
    desktop
};

std::string toString(Type type)
{
    switch (type)
    {
    case Type::desktop:
        return "desktop";
    case Type::mobile:
        return "mobile";
    default:
        throw std::invalid_argument("Invalid device type!");
    }
}

class Device
{
private:
    static int counter;
    const int id;
    char *name; // can be a nullptr

    void free()
    {
        delete[] name;
    }

protected:
    void copyPar(const char *src, char *dest)
    {
        if (src != nullptr)
            strcpy(dest, src);
        else
            dest = nullptr;
    }

    void setterHelper(const char *dest, const char *src)
    {
        char *temp = src ? new char[strlen(src) + 1] : nullptr;
        if (src != nullptr)
            strcpy(temp, src);
        delete[] dest;
        dest = temp;
    }

    void swap(Device &other) noexcept
    {
        std::swap(this->name, other.name);
    }

public:
    // DYN MEM ==> RO3
    // Def constr
    Device() : name(nullptr), id(++counter) {}
    // Par constr
    Device(const char *newName) : name(nullptr), id(++counter)
    {
        copyPar(newName, this->name);
    }
    // Operator =
    Device &operator=(const Device &other) = delete;
    // Destructor
    virtual ~Device()
    {
        free();
    }
    // GETTERS + SETTERS
    const char *getName() const
    {
        return this->name;
    }
    void setName(const char *newName)
    {
        setterHelper(this->name, newName);
    }
    int getId() const
    {
        return this->id;
    }
    // Virtual functions
    virtual const char *printString() const = 0;
    virtual void printSpecificInfo() const = 0;
    virtual Device *clone() const = 0;
};

class MobileDevice : public Device
{
private:
    int battery; // 0-100

    bool hasValidBattery()
    {
        return this->battery >= 0 && this->battery <= 100;
    }

public:
    // NO DYN MEM ==> NO RO3
    //  Defaul constructor
    MobileDevice() : Device(), battery(0) {}
    // Param constructor
    MobileDevice(const int newBattery, const char *newName) : Device(newName), battery(newBattery) {}
    // GETTER + SETTER
    int getBattery() const
    {
        return this->battery;
    }
    void setBattery(const int newBattery)
    {
        this->battery = newBattery;
    }
    // VIRTUAL FUNCTIONS
    void printSpecificInfo() const override
    {
        std::cout << "[Mobile Device] name: " << getName() << ", ID: " << getId() << ", battery lvl: " << this->battery << std::endl;
    }
    const char *printString() const override
    {
        toString(Type::mobile);
    }
    MobileDevice *clone() const override
    {
        return new MobileDevice(*this);
    }
};

class DesktopDevice : public Device
{
private:
    char *urlAdress;

    void free()
    {
        delete[] urlAdress;
    }

    void swap(DesktopDevice &other)
    {
        this->Device::swap(other);
        std::swap(this->urlAdress, other.urlAdress);
    }

public:
    // Dyn Mem ==> RO3
    // Def constr
    DesktopDevice() : Device(), urlAdress(nullptr) {}
    // Par constr
    DesktopDevice(const char *newUrlAdress, const char *newName) : Device(newName), urlAdress(nullptr)
    {
        setterHelper(this->urlAdress, newUrlAdress);
    }
    // Copy constr
    DesktopDevice(const DesktopDevice &other) : Device(other), urlAdress(nullptr)
    {
        setterHelper(this->urlAdress, other.urlAdress);
    }
    // Operator=
    DesktopDevice &operator=(DesktopDevice other)
    {
        this->swap(other);
        return *this;
    }
    // GETTERS + SETTERS
    const char *getUrlAdress() const
    {
        return this->urlAdress;
    }
    void setUrlAdress(const char *newUrlAdress)
    {
        setterHelper(this->urlAdress, newUrlAdress);
    }
    // VIRTUAL FUNCTIONS
    void printSpecificInfo() const override
    {
        std::cout << "[Desktop Device] name: " << getName() << ", ID: " << getId() << ", url: " << this->urlAdress << std::endl;
    }
    const char *printString() const override
    {
        toString(Type::desktop);
    }
    DesktopDevice *clone() const override
    {
        return new DesktopDevice(*this);
    }
};
