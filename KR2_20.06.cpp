#include <iostream>
#include <cstring>
#include <utility>
#include <stdexcept>

const size_t POLICIES_CAP = 200;

enum class EnsuranseType
{
    CARGO_CAR,
    PROPERTY_FIRE,
    ELECTRONIC_EQUIPMENT,
    PROPERTY_AND_EQUIPMENT
};

class Policy
{
    //  Abstract base class
private:
    char *productDescription;
    double ensuranceCost;
    double paidAmount;
    EnsuranseType policyType;

    void free()
    {
        delete[] productDescription;
    }

protected:
    void copyPar(const char *otherDesc)
    {

        this->productDescription = otherDesc ? new char[strlen(otherDesc) + 1] : nullptr;
        if (otherDesc != nullptr)
            strcpy(this->productDescription, otherDesc);
    }

    void swap(Policy &other) noexcept
    {
        std::swap(this->productDescription, other.productDescription);
        std::swap(this->ensuranceCost, other.ensuranceCost);
        std::swap(this->paidAmount, other.paidAmount);
        std::swap(this->policyType, other.policyType);
    }

    void setterHelper(const char *src, const char *dest)
    {
        char *temp = src ? new char[strlen(src) + 1] : nullptr;
        if (src != nullptr)
            strcpy(temp, src);
        delete[] dest;
        dest = temp;
    }

public:
    // Dyn Mem ==> RO3
    // Default constructor
    Policy() : productDescription(nullptr), ensuranceCost(0.0), paidAmount(0.0), policyType() {}
    // Param constructor
    Policy(const char *productDescr, const double ensurance, const double paid, const EnsuranseType type) : ensuranceCost(ensurance), paidAmount(paid), policyType(type)
    {
        copyPar(productDescr);
    }
    // Copy constructor
    Policy(const Policy &other) : ensuranceCost(other.ensuranceCost), paidAmount(other.paidAmount), policyType(other.policyType)
    {
        copyPar(other.productDescription);
    }

    Policy &operator=(const Policy &other) = delete;
    // Virtual Destructor
    virtual ~Policy()
    {
        free();
    }

    // GETTERS + SETTERS
    const char *getProductDescription() const
    {
        return productDescription;
    }
    void setProductDescription(const char *newDescription)
    {
        setterHelper(newDescription, this->productDescription);
    }
    const double getEnsuranceCost() const
    {
        return this->ensuranceCost;
    }
    void setEnsuranceCost(const double newEnsuranceCost)
    {
        this->ensuranceCost = newEnsuranceCost;
    }
    const double getPaidAmount() const
    {
        return this->paidAmount;
    }
    void setNewPaidAmount(const double newPaidAmount)
    {
        this->paidAmount = newPaidAmount;
    }
    const EnsuranseType getEnsuranceType() const
    {
        return this->policyType;
    }
    void setNewEnsuranceType(const EnsuranseType newEnsuranceType)
    {
        this->policyType = newEnsuranceType;
    }

    // VIRTUAL FUNCTIONS
    virtual void print() const
    {
        std::cout << "Description: " << (productDescription ? productDescription : "None")
                  << ", Insurance Cost: " << ensuranceCost
                  << ", Paid Amount: " << paidAmount;
    }
    virtual double paidSum() const = 0;
    virtual Policy *clone() const = 0;
};

class CargoCarPolicy : public Policy
{
private:
    char *carBrand;
    double cargoWeight;
    double maxWeight;
    char *path;

    void free()
    {
        delete[] carBrand;
        delete[] path;
    }

protected:
    void copyObj(const char *br, const char *way)
    {
        char *tempBr = nullptr;
        char *tempWay = nullptr;
        try
        {
            if (br != nullptr)
            {
                tempBr = new char[strlen(br) + 1];
                strcpy(tempBr, br);
            }
            if (way != nullptr)
            {
                tempWay = new char[strlen(way) + 1];
                strcpy(tempWay, way);
            }
        }
        catch (...)
        {
            delete[] tempBr;
            delete[] tempWay;
            // this means that one of the two chars is a nullptr, so we make no changes to the paramaters of the object
            throw;
        }
        delete[] this->carBrand;
        this->carBrand = tempBr;
        delete[] this->path;
        this->path = tempWay;
    }

    void swap(CargoCarPolicy &other) noexcept
    {
        this->Policy::swap(other);
        std::swap(this->carBrand, other.carBrand);
        std::swap(this->cargoWeight, other.cargoWeight);
        std::swap(this->maxWeight, other.maxWeight);
        std::swap(this->path, other.path);
    }

    void setterHelperTuples(const char *src1, const char *dest1, const char *src2, const char *dest2)
    {
        char *dest1Copy = dest1 ? new char[strlen(dest1) + 1] : nullptr;
        if (dest1 != nullptr)
            strcpy(dest1Copy, dest1);

        try
        {
            setterHelper(src1, dest1);
            try
            {
                setterHelper(src2, dest2);
            }
            catch (...)
            {
                setterHelper(dest1Copy, dest1);
                delete[] dest1Copy;
                throw;
            }
        }
        catch (...)
        {
            delete[] dest1Copy;
            throw;
        }
    }

public:
    // Dyn Mem ==> RO3
    // Default constructor
    CargoCarPolicy() : carBrand(nullptr), cargoWeight(0), maxWeight(0), path(nullptr) {}
    // Parametirized constructor
    CargoCarPolicy(const char *brand, const double cargo, const double maximumWeight, const char *way, const char *productDescr, const double insuranceCost, const double paid, const EnsuranseType type) : Policy(productDescr, insuranceCost, paid, type), cargoWeight(cargo), maxWeight(maximumWeight), carBrand(nullptr), path(nullptr)
    {
        copyObj(brand, way);
    }
    // Copy constructor
    CargoCarPolicy(const CargoCarPolicy &other) : Policy(other), carBrand(nullptr), path(nullptr)
    {
        copyObj(other.carBrand, other.path);
    }
    // Operator = using copy and swap
    CargoCarPolicy &operator=(CargoCarPolicy other)
    {
        this->swap(other);
        return *this;
    }
    // Destructor
    ~CargoCarPolicy()
    {
        free();
    }

    // GETTERS + SETTERS
    const char *getBrand() const
    {
        return this->carBrand;
    }
    void setBrand(const char *newBrand)
    {
        setterHelper(newBrand, this->carBrand);
    }
    const double getCargoWeight() const
    {
        return this->cargoWeight;
    }
    void setCargoWeight(const double newCargoWeight)
    {
        this->cargoWeight = newCargoWeight;
    }
    const double getMaxWeight() const
    {
        return this->maxWeight;
    }
    void setMaxWeight(const double newMaxWeight)
    {
        this->maxWeight = newMaxWeight;
    }
    const char *getPathway() const
    {
        return this->path;
    }
    void setPathway(const char *newPathway)
    {
        setterHelper(newPathway, this->path);
    }

    // Virtual functions
    void print() const override
    {
        Policy::print();
        std::cout << " -> CargoCar [ Brand: " << (carBrand ? carBrand : "None")
                  << ", Value/Weight: " << cargoWeight
                  << ", MaxWeight: " << maxWeight
                  << ", Path: " << (path ? path : "None") << " ]\n";
    }
    double paidSum() const override
    {
        if (cargoWeight <= 5)
            return cargoWeight;
        return (3.0 / 4.0) * cargoWeight;
    }
    CargoCarPolicy *clone() const override
    {
        return new CargoCarPolicy(*this);
    }
};

class PropertyFirePolicy : virtual public Policy
{
private:
    double propertyCost;
    double area;
    int yearBuild;
    bool hasFireSystem;

protected:
    void swapOwnData(PropertyFirePolicy &other) noexcept
    {
        std::swap(this->propertyCost, other.propertyCost);
        std::swap(this->area, other.area);
        std::swap(this->yearBuild, other.yearBuild);
        std::swap(this->hasFireSystem, other.hasFireSystem);
    }

public:
    // No dyn mem ==> no ro3
    // Default constructor
    PropertyFirePolicy() : Policy(), propertyCost(0), area(0), yearBuild(0), hasFireSystem(false) {}
    // Par constructor
    PropertyFirePolicy(double newPropertyCost, double newArea, int newYear, bool newHasFireSystem,
                       const char *descr, double cost, double paid, EnsuranseType type)
        : Policy(descr, cost, paid, type), // Вика се, ако правим САМО PropertyFirePolicy
          propertyCost(newPropertyCost), area(newArea), yearBuild(newYear), hasFireSystem(newHasFireSystem)
    {
    }

    // GETTERS + SETTERS
    const double getPropertyCost() const
    {
        return this->propertyCost;
    }
    void setPropertyCost(const double newPropertyCost)
    {
        this->propertyCost = newPropertyCost;
    }
    const double getArea() const
    {
        return this->area;
    }
    void setArea(const double newArea)
    {
        this->area = newArea;
    }
    const int getYearBuild() const
    {
        return this->yearBuild;
    }
    void setYearBuild(const int newYearBuild)
    {
        this->yearBuild = newYearBuild;
    }
    const bool getHasFireSystem() const
    {
        return hasFireSystem ? true : false;
    }
    void setHasFireSystem(const bool newHasFireSystem)
    {
        this->hasFireSystem = newHasFireSystem;
    }

    // VIRTUAL FUNCTIONS
    void print() const override
    {
        Policy::print();
        std::cout << " -> PropertyFire [ Cost: " << propertyCost
                  << ", Area: " << area
                  << ", Built: " << yearBuild
                  << ", Fire System: " << (hasFireSystem ? "Yes" : "No") << " ]\n";
    }

    double paidSum() const override
    {
        // Пресмятане спрямо текущата година на изпита (2026)
        int age = 2026 - yearBuild;
        if (hasFireSystem)
        {
            if (age <= 25)
                return propertyCost;    // 100%
            return 0.75 * propertyCost; // 75%
        }
        return 0.50 * propertyCost; // 50%
    }
    PropertyFirePolicy *clone() const override
    {
        return new PropertyFirePolicy(*this);
    }
};

class ElectronicEquipmentPolicy : virtual public Policy
{
private:
    char *technologyType;
    int yearBuild;
    double technoValue;

    void free()
    {
        delete[] technologyType;
    }

protected:
    void copyPar(const char *technoType)
    {
        if (technoType == nullptr)
        {
            this->technologyType = nullptr;
            return;
        }
        this->technologyType = new char[strlen(technoType) + 1];
        strcpy(this->technologyType, technoType);
    }
    void swapOwnData(ElectronicEquipmentPolicy &other) noexcept
    {
        std::swap(this->technologyType, other.technologyType);
        std::swap(this->yearBuild, other.yearBuild);
        std::swap(this->technoValue, other.technoValue);
    }
    void swap(ElectronicEquipmentPolicy &other) noexcept
    {
        this->Policy::swap(other);
        swapOwnData(other);
    }

public:
    // DYN MEM ==> BIG 5
    // Default constructor
    ElectronicEquipmentPolicy() : Policy(), technologyType(nullptr), yearBuild(0), technoValue(0) {}
    // Par Constructor
    ElectronicEquipmentPolicy(const char *newTechnologyType, const int newYearBuild, const double newTechnoValue, const char *productDescr, const double insuranceCost, const double paid, const EnsuranseType type) : Policy(productDescr, insuranceCost, paid, type), yearBuild(newYearBuild), technoValue(newTechnoValue)
    {
        copyPar(newTechnologyType);
    }
    // Copy Constructor
    ElectronicEquipmentPolicy(const ElectronicEquipmentPolicy &other) : Policy(other), yearBuild(other.yearBuild), technoValue(other.technoValue)
    {
        copyPar(other.technologyType);
    }
    // Operator =
    ElectronicEquipmentPolicy &operator=(ElectronicEquipmentPolicy other)
    {
        this->swap(other);
        return *this;
    }
    ~ElectronicEquipmentPolicy()
    {
        free();
    }
    // GETTERS + SETTERS
    const int getYearBuild() const
    {
        return this->yearBuild;
    }
    void setYearBuild(const int newYearBuild)
    {
        this->yearBuild = newYearBuild;
    }
    const double getTechnoValue() const
    {
        return this->technoValue;
    }
    void setTechnoValue(const double newTechnoValue)
    {
        this->technoValue = newTechnoValue;
    }
    const char *getTechnologyType() const
    {
        return this->technologyType;
    }
    void setTechnologyType(const char *newTechnologyType)
    {
        setterHelper(newTechnologyType, this->technologyType);
    }
    // Virtual functions
    double paidSum() const override
    {
        return technoValue;
    }
    ElectronicEquipmentPolicy *clone() const override
    {
        return new ElectronicEquipmentPolicy(*this);
    }
    void print() const override
    {
        Policy::print();
        std::cout << " -> ElectronicEquipment [ Tech Type: " << (technologyType ? technologyType : "None")
                  << ", Manufactured: " << yearBuild
                  << ", Value: " << technoValue << " ]" << std::endl;
    }
};

class PropertyAndEquipmentPolicy : public PropertyFirePolicy, public ElectronicEquipmentPolicy
{
private:
    int combinedRisk;
    double combinedAmountStocks;

protected:
    void swap(PropertyAndEquipmentPolicy &other) noexcept
    {
        this->Policy::swap(other);
        this->PropertyFirePolicy::swapOwnData(other);
        this->ElectronicEquipmentPolicy::swapOwnData(other);
        std::swap(this->combinedRisk, other.combinedRisk);
        std::swap(this->combinedAmountStocks, other.combinedAmountStocks);
    }

public:
    // NO DYN MEMORY
    // Default Constructor
    PropertyAndEquipmentPolicy() : PropertyFirePolicy(), ElectronicEquipmentPolicy(), combinedRisk(0), combinedAmountStocks(0) {}
    // Paramaterized constructor
    PropertyAndEquipmentPolicy(const int newCombinedRisk, const double newCombinedAmountStocks,
                               const char *newTechnologyType, const int newYearBuild, const double newTechnoValue,
                               const char *productDescr, const double ensuranceCost, const double paid, const EnsuranseType type,
                               const double propertyCostPar, const double areaPar, const int yearBuildPar, const bool hasFireSystemPar) : Policy(productDescr, ensuranceCost, paid, type), ElectronicEquipmentPolicy(newTechnologyType, newYearBuild, newTechnoValue, productDescr, ensuranceCost, paid, type), PropertyFirePolicy(propertyCostPar, areaPar, yearBuildPar, hasFireSystemPar, productDescr, ensuranceCost, paid, type),
                                                                                                                                          combinedRisk(newCombinedRisk), combinedAmountStocks(newCombinedAmountStocks) {}

    // Copy constructor
    PropertyAndEquipmentPolicy(const PropertyAndEquipmentPolicy &other)
        : Policy(other), PropertyFirePolicy(other), ElectronicEquipmentPolicy(other), combinedRisk(other.combinedRisk), combinedAmountStocks(other.combinedAmountStocks) {}
    // Operator =
    PropertyAndEquipmentPolicy &operator=(PropertyAndEquipmentPolicy other)
    {
        this->swap(other);
        return *this;
    }
    // Destructor
    ~PropertyAndEquipmentPolicy() = default;
    // GETTERS + SETTERS
    void setCombinedRisk(const int newCombinedRisk)
    {
        if (newCombinedRisk >= 0 && newCombinedRisk <= 100)
            this->combinedRisk = newCombinedRisk;
        else
        {
            throw std::invalid_argument("Invalid percentage value!");
        }
    }
    const int getCombinedRisk() const
    {
        return this->combinedRisk;
    }
    void setCombinedAmountStocks(const double newCombinedAmountStocks)
    {
        this->combinedAmountStocks = newCombinedAmountStocks;
    }
    const double getCombinedAmountStocks() const
    {
        return this->combinedAmountStocks;
    }

    // VIRTUAL FUNCTIONS
    double paidSum() const override
    {
        return (combinedRisk / 100.0) * combinedAmountStocks;
    }

    PropertyAndEquipmentPolicy *clone() const override
    {
        return new PropertyAndEquipmentPolicy(*this);
    }
    void print() const override
    {
        Policy::print();
        std::cout << " -> Combined Policy [ Combined Risk: " << combinedRisk << "%"
                  << ", Combined Assets Value: " << combinedAmountStocks << " ]" << std::endl;
    }
};

class BullFMI
{
private:
    Policy **policies;
    size_t count;
    size_t capacity;
    double availableFunds; // Пази парите на компанията

    void free()
    {
        for (size_t i = 0; i < count; ++i)
        {
            delete policies[i]; // Трием всеки полиморфен обект
        }
        delete[] policies; // Трием масива от указатели
    }

    void copyFrom(const BullFMI &other)
    {
        this->capacity = other.capacity;
        this->count = other.count;
        this->availableFunds = other.availableFunds;

        this->policies = new Policy *[this->capacity];
        for (size_t i = 0; i < this->count; ++i)
        {
            this->policies[i] = other.policies[i]->clone(); // Дълбоко копиране чрез Prototype Pattern
        }
    }

    void resize()
    {
        this->capacity *= 2;
        Policy **temp = new Policy *[this->capacity];
        for (size_t i = 0; i < this->count; ++i)
        {
            temp[i] = this->policies[i]; // Прехвърляме указателите
        }
        delete[] this->policies; // Трием стария масив (но НЕ и обектите, към които сочат указателите!)
        this->policies = temp;
    }

public:
    // 1. Конструктори и Деструктор (Задължителни!)
    BullFMI() : policies(new Policy *[3]), count(0), capacity(4), availableFunds(10000.0) {} // Примерни начални пари

    BullFMI(const BullFMI &other)
    {
        copyFrom(other);
    }

    BullFMI &operator=(BullFMI other)
    {
        // Можеш да си напишеш бърз swap метод за BullFMI и да ползваш copy-and-swap
        std::swap(this->policies, other.policies);
        std::swap(this->count, other.count);
        std::swap(this->capacity, other.capacity);
        std::swap(this->availableFunds, other.availableFunds);
        return *this;
    }

    ~BullFMI()
    {
        free();
    }

    // 2. Бизнес логика (Премести addPolicy и removePolicy в public секцията!)
    void addPolicy(const Policy &newPol)
    {
        // ТУК трябва да добавиш проверка дали парите на компанията стигат за покриване
        // Ако сумата (ensuranceCost) надхвърля availableFunds, може да хвърлиш изключение

        if (this->count >= capacity)
            resize();

        policies[count] = newPol.clone();
        count++;
    }

    void removePolicy(size_t index)
    {
        if (index >= count)
        {
            throw std::out_of_range("Invalid index");
        }
        delete policies[index];
        for (size_t i = index; i < (count - 1); ++i)
        {
            this->policies[i] = this->policies[i + 1];
        }
        count--;
    }

    // 3. Други задължителни методи (обикновено изисквани в това задание)
    void printAll() const
    {
        std::cout << "Company Funds: " << availableFunds << "\n";
        for (size_t i = 0; i < count; ++i)
        {
            policies[i]->print(); // Полиморфно извикване
        }
    }

    // Метод за изплащане на застраховка при събитие
    void executeClaim(size_t index)
    {
        if (index >= count)
            return;
        double payout = policies[index]->paidSum(); // Смята колко трябва да се плати
        if (availableFunds >= payout)
        {
            availableFunds -= payout;
            std::cout << "Paid " << payout << " for policy at index " << index << "\n";
        }
        else
        {
            std::cout << "Bankrupt! Insufficient funds to execute claim.\n";
        }
    }
};

int main()
{
    //// Полица за карго автомобил (тегло 4.5 тона <= 5 тона -> paidSum() трябва да върне 4.5)
    // CargoCarPolicy cargoPol("Volvo", 4.5, 12.0, "Sofia-Burgas",
    //                         "Cargo Insurance", 500.0, 500.0, EnsuranseType::CARGO_CAR);
    CargoCarPolicy carPol("Volvo", 3.0, 10.0, "Sofia~>Bali~>Paris~>Sofia",
                          "Car Insurance", 40000, 10000, EnsuranseType::CARGO_CAR);

    PropertyFirePolicy propertyPol(1000000, 200, 1934, true,
                                   "Property Insurance", 700000, 300000, EnsuranseType::PROPERTY_FIRE);

    ElectronicEquipmentPolicy electronicPol("Kalashnikov", 1953, 3000,
                                            "Equipment Policy", 2500, 1500, EnsuranseType::ELECTRONIC_EQUIPMENT);

    PropertyAndEquipmentPolicy mixedPol(71, 3000000,
                                        "Iphone 1", 2005, 10000,
                                        "Mixed Insurance", 10000, 7000, EnsuranseType::PROPERTY_AND_EQUIPMENT,
                                        750000, 100, 2010, false);

    BullFMI pimp;
    pimp.addPolicy(carPol);
    pimp.addPolicy(propertyPol);
    pimp.addPolicy(electronicPol);
    std::cout << "Successfully added 3 policies to BullFMI" << std::endl;
    // adding a fourth policy -> expecting automatic resize
    pimp.addPolicy(mixedPol);
    std::cout << "Successfully resized BullFMI and added a fourth policy " << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "Printing all policies: " << std::endl;
    pimp.printAll();
    pimp.removePolicy(0);
    pimp.removePolicy(2);
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "Printing all policies after removing previous policies at indexes 0 and 2" << std::endl;
    pimp.printAll();

    return 0;
}