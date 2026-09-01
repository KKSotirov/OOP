#include <iostream>
#include <utility>
#include <cstring>
#include <fstream>

enum class DeviceType
{
    mobile,
    desktop
};

class Device
{
private:
    const int id;
    static int counter;
    char *name;

    void free()
    {
        delete[] name;
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

    void swap(Device &other)
    {
        std::swap(this->name, other.name);
    }

public:
    // RO3

    // Def constructor
    Device() : id(++counter), name(nullptr) {}

    // Par constructor
    Device(const char *newName) : id(++counter), name(nullptr)
    {
        setterHelper(name, newName);
    }

    // Copy constructor
    Device(const Device &other) : id(++counter), name(nullptr)
    {
        setterHelper(name, other.name);
    }

    // Operator =
    Device &operator=(const Device &other)
    {
        if (this != &other)
            setterHelper(this->name, other.name);

        return *this;
    }

    // Virtual Destructor
    virtual ~Device()
    {
        free();
    }

    // Getters + Setters
    const char *getName() const
    {
        return name;
    }

    void setName(const char *newName)
    {
        setterHelper(name, newName);
    }

    int getId() const
    {
        return id;
    }

    // Pure virtual functions
    virtual DeviceType getType() const = 0;
    virtual void print() const = 0;
    virtual Device *clone() const = 0;
};

int Device::counter = 0;

class MobileDevice : public Device
{
private:
    int batteryLevel; // 0-100

    bool isValidBattery(const int batteryLvl)
    {
        return (batteryLvl >= 0) && (batteryLvl <= 100);
    }

public:
    // No RO3

    // Default constructor
    MobileDevice() : batteryLevel(0), Device() {}

    // Par constructor
    MobileDevice(const char *newName, const int newBatteryLevel) : batteryLevel(newBatteryLevel), Device(newName) {}

    // Copy constructor
    MobileDevice(const MobileDevice &other) : Device(other)
    {
        if (isValidBattery(other.batteryLevel))
            this->batteryLevel = other.batteryLevel;
        else
            std::cerr << "Invalid battery level! \n";
    }

    // Getters + Setters
    int getBatteryLevel() const
    {
        return batteryLevel;
    }

    void setBatteryLevel(const int newBatteryLevel)
    {
        batteryLevel = newBatteryLevel;
    }

    // Virtual functions
    DeviceType getType() const override
    {
        return DeviceType::mobile;
    }

    MobileDevice *clone() const override
    {
        return new MobileDevice(*this);
    }

    void print() const override
    {
        std::cout << "[Mobile] " << getName() << " (ID: " << getId()
                  << "), Battery: " << batteryLevel << "\n";
    }
};

class DesktopDevice : public Device
{
private:
    char *urlAdress;

    bool isValidUrlAdress(const char *url)
    {
        return (url != nullptr) && strlen(url) > 0;
    }

    void free()
    {
        delete[] urlAdress;
    }

protected:
    void swap(DesktopDevice &other)
    {
        Device::swap(other);
        std::swap(this->urlAdress, other.urlAdress);
    }

public:
    // RO3

    // Default Constructor
    DesktopDevice() : urlAdress(nullptr), Device() {}

    // Par Constructor
    DesktopDevice(const char *newUrlAdress, const char *newName) : Device(newName), urlAdress(nullptr)
    {
        setterHelper(urlAdress, newUrlAdress);
    }

    // Copy Constructor
    DesktopDevice(const DesktopDevice &other) : Device(other)
    {
        if (isValidUrlAdress(other.urlAdress))
            setterHelper(this->urlAdress, other.urlAdress);
    }

    // Operator =
    DesktopDevice &operator=(DesktopDevice other)
    {
        this->swap(other);
        return *this;
    }

    // Destructor
    ~DesktopDevice()
    {
        free();
    }

    // Getters + Setters
    const char *getUrlAdress() const
    {
        return urlAdress;
    }

    void setUrlAdress(const char *newUrlAdress)
    {
        if (isValidUrlAdress(newUrlAdress))
            setterHelper(urlAdress, newUrlAdress);
        else
            std::cerr << "Invalid url Adress! \n";
    }

    // Virtual functions

    DeviceType getType() const override
    {
        return DeviceType::desktop;
    }

    DesktopDevice *clone() const override
    {
        return new DesktopDevice(*this);
    }

    void print() const override
    {
        std::cout << "[Desktop] " << getName() << " (ID: " << getId()
                  << "), URL: " << urlAdress << "\n";
    }
};

class Connection
{
private:
    Device *device;
    int connectedAt;

    // Par constructor
    friend class Server;
    Connection(Device *newDevice, const int newConnectedAt) : device(newDevice), connectedAt(newConnectedAt) {}

    // Forbid copying
    Connection(const Connection &other) = delete;
    Connection &operator=(const Connection &other) = delete;

    // Destructor
    ~Connection()
    {
        delete device;
    }

public:
    // Getters+ Setters
    Device *getDevice() const
    {
        return device;
    }

    int getConnectedAt() const
    {
        return connectedAt;
    }
};

class Server
{
private:
    Connection **connections;
    unsigned capacity;
    unsigned size;
    unsigned timeout = 0;
    unsigned currentTime;

    void free()
    {
        for (size_t i = 0; i < size; i++)
        {
            delete connections[i];
        }
        delete[] connections;
    }

public:
    Server(const unsigned newCapacity, const unsigned newTimeout) : capacity(newCapacity), timeout(newTimeout), size(0), currentTime(0)
    {
        connections = new Connection *[capacity];
    }

    bool connect(const Device &newDevice)
    {
        if (size >= capacity)
            return false;

        connections[++size] = new Connection(newDevice.clone(), currentTime);
        return true;
    }

    bool disconnect(const int deviceId)
    {
        int index = -1;

        for (size_t i = 0; i < size; i++)
        {
            if (connections[i]->device->getId() == deviceId)
                index = i;
        }

        if (index == -1)
            return false;

        delete connections[index];

        for (size_t i = index; i < size - 1; i++)
        {
            connections[i] = connections[i + 1];
        }

        connections[--size] = nullptr;

        return true;
    }

    bool tick(const unsigned count)
    {
        currentTime += count;

        for (size_t i = 0; i < count; i++)
        {
            if ((currentTime - connections[i]->getConnectedAt()) >= timeout)
                disconnect(connections[i]->device->getId());
        }
    }
};