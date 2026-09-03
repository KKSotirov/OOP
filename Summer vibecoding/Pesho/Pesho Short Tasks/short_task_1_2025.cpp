#include <iostream>
#include <cstring>

enum class ArchitectureType
{
    x86_64,
    ARM,
    RISC_V,
    MIPS,
    Other_RISC,
    Other_CISC
};

class CalculatingDevice
{
private:
    char *brand;
    char *model;
    double freq;
    ArchitectureType type;

    void free()
    {
        delete[] brand;
        brand = nullptr;
        delete[] model;
        model = nullptr;
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
    CalculatingDevice() : brand(nullptr), model(nullptr), freq(0.0), type(ArchitectureType::x86_64) {}
    CalculatingDevice(const char *_brand, const char *_model, const double _freq, const ArchitectureType _type) : brand(nullptr), model(nullptr), freq(_freq), type(_type)
    {
        setBrand(_brand);
        setModel(_model);
    }

    CalculatingDevice(const CalculatingDevice &other) : brand(nullptr), model(nullptr), freq(other.freq), type(other.type)
    {
        setBrand(other.brand);
        setModel(other.model);
    }

    CalculatingDevice &operator=(const CalculatingDevice &other)
    {
        if (this != &other)
        {
            free();
            setBrand(other.brand);
            setModel(other.model);
            freq = other.freq;
            // type = other.type;    type is const!
        }
        return *this;
    }

    virtual ~CalculatingDevice()
    {
        free();
    }

    // Getters + Setters
    const char *getBrand() const
    {
        return brand;
    }

    void setBrand(const char *_brand)
    {
        setterHelper(brand, _brand);
    }

    const char *getModel() const
    {
        return model;
    }

    void setModel(const char *_model)
    {
        setterHelper(model, _model);
    }

    // No setter for architecture type
    ArchitectureType getType() const
    {
        return type;
    }

    double getFreq() const
    {
        return freq;
    }

    void setFreq(const double _freq)
    {
        freq = _freq;
    }

    // Virtual functions
    virtual CalculatingDevice *clone() const = 0;
    virtual unsigned compute(const unsigned *arr) const = 0;
};

class Laptop : public CalculatingDevice
{
private:
    unsigned mem;
    unsigned hdd;

public:
    // No dyn mem -> no r03
    Laptop(const char *_brand, const char *_model, const double _freq, const ArchitectureType _type, const unsigned _mem, const unsigned _hdd) : CalculatingDevice(_brand, _model, _freq, ((_type == ArchitectureType::ARM) || (_type == ArchitectureType::x86_64) ? _type : ArchitectureType::ARM)), mem(_mem), hdd(_hdd) {}

    // Getters + Setters
    unsigned getMem() const
    {
        return mem;
    }

    void setMem(const unsigned _mem)
    {
        mem = _mem;
    }

    unsigned getHdd() const
    {
        return hdd;
    }

    void setHdd(const unsigned _hdd)
    {
        hdd = _hdd;
    }

    // Virtual functions
    Laptop *clone() const override
    {
        return new Laptop(*this);
    }

    unsigned compute(const unsigned *arr) const override
    {
        unsigned sum = 0;
        if (mem > sizeof(arr[0]) * 8)
        {
            for (size_t i = 0; i < 8; i++)
            {
                sum += arr[i];
            }
        }
        return sum;
    }
};

class IoT : public CalculatingDevice
{
private:
    char *sensors;

    void free()
    {
        delete[] sensors;
        sensors = nullptr;
    }

    bool hasMin_Calc() const
    {
        if (sensors == nullptr)
            return false;

        const char *ptr = sensors;
        char *tmp = new char[strlen(sensors) + 1];
        int lenCurrWord = 0;

        while (*ptr)
        {
            // Lets take each word one at a time
            if (*ptr != ' ')
            {
                while (*ptr && *ptr != ' ')
                {
                    tmp[lenCurrWord] = *ptr;
                    lenCurrWord++;
                    ptr++;
                }
                tmp[lenCurrWord] = '\0';
                if (strcmp(tmp, "min-calc") == 0)
                {
                    delete[] tmp;
                    tmp = nullptr;
                    return true;
                }
                lenCurrWord = 0;
            }
            else
                ptr++;
        }
        delete[] tmp;
        tmp = nullptr;
        return false;
    }

public:
    IoT(const char *_brand, const char *_model, const double _freq, const ArchitectureType _type, const char *_sensors) : CalculatingDevice(_brand, _model, _freq, _type), sensors(nullptr)
    {
        setSensors(_sensors);
    }

    IoT(const IoT &other) : CalculatingDevice(other), sensors(nullptr)
    {
        setSensors(other.sensors);
    }

    IoT &operator=(const IoT &other)
    {
        if (this != &other)
        {
            free();
            this->CalculatingDevice::operator=(other);
            setSensors(other.sensors);
        }
        return *this;
    }

    ~IoT() override
    {
        free();
    }

    // Getters + Setters
    const char *getSensors() const
    {
        return sensors;
    }

    void setSensors(const char *_sensors)
    {
        setterHelper(sensors, _sensors);
    }

    // Virtual functions:
    IoT *clone() const override
    {
        return new IoT(*this);
    }

    unsigned compute(const unsigned *arr) const override
    {
        unsigned minVal = arr[0];
        if (this->hasMin_Calc())
        {
            for (size_t i = 0; i < 8; i++)
            {
                if (arr[i] < minVal)
                    minVal = arr[i];
            }
            return minVal;
        }
        return 0;
    }
};

class ComputingCenter
{
private:
    CalculatingDevice **devices;
    size_t capacity;
    size_t count;

    void free()
    {
        for (size_t i = 0; i < count; i++)
        {
            delete devices[i];
        }
        delete[] devices;
    }

    void resize()
    {
        if (capacity == 16)
        {
            std::cout << "Maximum size of container has been reached! \n";
            return;
        }
        capacity *= 2;
        CalculatingDevice **tmp = new CalculatingDevice *[capacity];
        for (size_t i = 0; i < count; i++)
        {
            tmp[i] = devices[i];
        }
        delete[] devices;
        devices = tmp;
    }

    void tighten(const size_t indexOfDeleted)
    {
        for (size_t i = indexOfDeleted; i < count - 1; i++)
        {
            devices[i] = devices[i + 1];
        }
        count--;
    }

public:
    ComputingCenter() : capacity(2), count(0)
    {
        devices = new CalculatingDevice *[capacity];
    }

    ComputingCenter(const ComputingCenter &other) = delete;
    ComputingCenter &operator=(const ComputingCenter &other) = delete;

    ~ComputingCenter()
    {
        free();
    }

    bool addDevice(const CalculatingDevice *_device)
    {
        if (count >= capacity)
            resize();
        if (count >= capacity)
            return false; // Resize didn't work ~~> we have reached our max container size

        devices[count++] = _device->clone();
        return true;
    }

    bool removeDevice(const char *_brand)
    {
        bool deletedASingleItem = false;
        for (size_t i = 0; i < count; i++)
        {
            if (strcmp(devices[i]->getBrand(), _brand) == 0)
            {
                // we delete this phone
                delete devices[i];
                tighten(i);
                i--;
                deletedASingleItem = true;
            }
        }
        return deletedASingleItem;
    }

    void compute(const unsigned *arr) const
    {
        for (size_t i = 0; i < count; i++)
        {
            std::cout << " Device " << devices[i]->getBrand()
                      << ", model " << devices[i]->getModel()
                      << " has compute = " << devices[i]->compute(arr) << "\n";
        }
    }
};