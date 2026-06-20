#include <iostream>
#include <utility>
#include <cstring>

class Device
{
private:
    const int id;
    static int counter;

protected:
    char *brand;

    void copy(const Device &other)
    {
        if (other.brand == nullptr)
            this->brand = nullptr;
        else
        {
            this->brand = new char[strlen(other.brand) + 1];
            strcpy(this->brand, other.brand);
        }
    }

    void swap(Device &other) noexcept
    {
        std::swap(this->brand, other.brand);
    }

    void free()
    {
        delete[] this->brand;
    }

public:
    // DYNAMIC MEMORY ---> R03
    Device(const char *str) : id(++counter)
    {
        if (str == nullptr)
            this->brand = nullptr;
        else
        {
            this->brand = new char[strlen(str) + 1];
            strcpy(this->brand, str);
        }
    }
    // copy constructor
    Device(const Device &other) : id(++counter)
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

    // VIRTUAL FUNCTIONS
    virtual Device *clone() const = 0;
    virtual void printSpecs() const = 0;
};

int Device::counter = 0;

class Laptop : public Device
{
private:
    int battery_life;

public:
    // NO DYN MEMORY --> NO RO3
    // WE NEED PARAMETERIZED CONSTR + DEFAULT
    Laptop(const char *br, const int battery) : Device(br), battery_life(battery) {}
    Laptop *clone() const override
    {
        return new Laptop(*this);
    }
    void printSpecs() const override
    {
        std::cout << "[LAPTOP], " << "Brand: " << this->brand << ", battery life: " << this->battery_life << std::endl;
    }
};

class Server : public Device
{
private:
    int rack_units;

public:
    // NO RO3
    Server(const char *br, const int units) : Device(br), rack_units(units) {}

    Server *clone() const override
    {
        return new Server(*this);
    }

    void printSpecs() const override
    {
        std::cout << "[SERVER], " << "Brand, " << this->brand << ", rack units: " << this->rack_units << std::endl;
    }
};

class Inventory
{
private:
    Device **devices;
    unsigned count;
    unsigned capacity;

    void copy(const Inventory &other)
    {
        this->count = other.count;
        this->capacity = other.capacity;
        if (other.devices == nullptr)
        {
            this->devices = nullptr;
            return;
        }
        this->devices = new Device *[this->capacity];
        for (size_t i = 0; i < count; i++)
        {
            if (other.devices[i] == nullptr)
            {
                this->devices[i] = nullptr;
            }
            else
            {
                this->devices[i] = other.devices[i]->clone();
            }
        }
    }

    void free()
    {
        for (size_t i = 0; i < count; i++)
        {
            delete this->devices[i];
        }
        delete[] this->devices;
    }

    void resize()
    {
        if (capacity <= count)
        {
            size_t newcap = (capacity == 0) ? 2 : capacity * 2;
            Device **resized = new Device *[newcap];
            for (size_t i = 0; i < capacity; i++)
            {
                resized[i] = this->devices[i];
            }
            delete[] devices;
            this->devices = resized;
            capacity = newcap;
        }
    }

    void swap(Inventory &other)
    {
        std::swap(this->capacity, other.capacity);
        std::swap(this->count, other.count);
        std::swap(this->devices, other.devices);
    }

public:
    // DYNAMIC MEMORY --> RO3
    // PARAMETRIZED CONSTRUCTOR + DEFAULT CONSTRUCTOR
    Inventory() : devices(nullptr), count(0), capacity(0) {}
    Inventory(const unsigned cnt, const unsigned cap) : count(cnt), capacity(cap)
    {
        this->devices = new Device *[capacity];

        for (size_t i = 0; i < capacity; i++)
        {
            this->devices[i] = nullptr;
        }
    }
    // RO3
    // COPY CONSTRUCTOR
    Inventory(const Inventory &other)
    {
        copy(other);
    }

    Inventory &operator=(Inventory other)
    {
        this->swap(other);
        return *this;
    } // Bi li mi pokazal metoda na Armianov ako ti se zanimava

    ~Inventory()
    {
        free();
    }

    void addDevice(const Device &d)
    {
        if (count >= capacity)
        {
            resize();
        }
        this->devices[count] = d.clone();
        count++;
    }
};