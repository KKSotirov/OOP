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
    void copyPar(const char *src, char *&dest)
    {
        if (src != nullptr)
        {
            dest = new char[strlen(src) + 1];
            strcpy(dest, src);
        }
        else
            dest = nullptr;
    }

    void setterHelper(char *&dest, const char *src)
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
    // Copy constr
    Device(const Device &other) : id(++counter), name(nullptr)
    {
        copyPar(other.name, this->name);
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

// Фиксирано: Задължителна инициализация на статичния брояч за линкера
int Device::counter = 0;

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
        // Фиксирано: Защита от nullptr при принтиране на името
        std::cout << "[Mobile Device] name: " << (getName() ? getName() : "None")
                  << ", ID: " << getId() << ", battery lvl: " << this->battery << std::endl;
    }
    // Фиксирано: Добавен коректен return оператор
    const char *printString() const override
    {
        return "mobile";
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
        // Фиксирано: Защита от nullptr при принтиране
        std::cout << "[Desktop Device] name: " << (getName() ? getName() : "None")
                  << ", ID: " << getId() << ", url: " << (this->urlAdress ? this->urlAdress : "None") << std::endl;
    }
    // Фиксирано: Добавен коректен return оператор
    const char *printString() const override
    {
        return "desktop";
    }
    DesktopDevice *clone() const override
    {
        return new DesktopDevice(*this);
    }
};

class Connection
{
private:
    Device *connectedWith;
    int startOfConnection;
    friend class Server;

    Connection(const Device &connectedDevice, const int start) : startOfConnection(start)
    {
        this->connectedWith = connectedDevice.clone();
    }

    // Фиксирано: Инициализираме connectedWith(nullptr) за безопасност
    Connection(const Connection &other) : startOfConnection(other.startOfConnection), connectedWith(nullptr)
    {
        if (other.connectedWith != nullptr)
            this->connectedWith = other.connectedWith->clone();
    }

    void free()
    {
        delete connectedWith;
    }

public:
    // Направено const за по-добра съвместимост с константни обекти
    Connection &operator=(const Connection &other)
    {
        if (this != &other)
        {
            Device *tempObj = other.connectedWith ? other.connectedWith->clone() : nullptr;
            free();
            this->connectedWith = tempObj;
            this->startOfConnection = other.startOfConnection;
        }
        return *this;
    }
    // GETTERS + SETTERS
    void setStartOfConnection(const int newStartOfConnection)
    {
        this->startOfConnection = newStartOfConnection;
    }
    int getStartOfConnection() const
    {
        return this->startOfConnection;
    }

    ~Connection()
    {
        free();
    }
};

class Server
{
private:
    Connection **connections;
    size_t timeout = 0;
    size_t count;
    size_t capacity;

    void resize()
    {
        capacity *= 2;
        Connection **tempConnections = new Connection *[capacity];
        for (size_t i = 0; i < count; i++)
        {
            tempConnections[i] = this->connections[i];
        }
        delete[] this->connections;
        this->connections = tempConnections;
    }

    void free()
    {
        for (size_t i = 0; i < count; i++)
        {
            delete this->connections[i];
        }
        delete[] this->connections;
    }

public:
    Server(unsigned newCapacity, unsigned newTime) : capacity(newCapacity), timeout(newTime), count(0)
    {
        this->connections = new Connection *[capacity];
    }

    bool connect(const Device &d)
    {
        if (this->count >= capacity)
            resize();
        this->connections[count] = new Connection(d, 0);
        this->count++; // Фиксирано: count++, а не connections++!
        return true;
    }

    ~Server()
    {
        free();
    }
};