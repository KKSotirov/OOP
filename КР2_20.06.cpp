#include <iostream>
#include <cstring>
#include <utility>

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
        if (otherDesc == nullptr)
        {
            this->productDescription = nullptr;
            return;
        }
        this->productDescription = new char[strlen(otherDesc) + 1];
        strcpy(this->productDescription, otherDesc);
    }

    void swap(Policy &other) noexcept
    {
        std::swap(this->productDescription, other.productDescription);
        std::swap(this->ensuranceCost, other.ensuranceCost);
        std::swap(this->paidAmount, other.paidAmount);
        std::swap(this->policyType, other.policyType);
    }

public:
    // Dyn Mem ==> RO3
    // Default constructor
    Policy() : productDescription(nullptr), ensuranceCost(0), paidAmount(0), policyType() {}
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
    // Operator = with copy and swap
    Policy &operator=(Policy other)
    {
        this->swap(other);
        return *this;
    }
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
        char *temp = newDescription ? new char[strlen(newDescription) + 1] : nullptr;
        if (newDescription != nullptr)
        {
            strcpy(temp, newDescription);
        }
        delete[] this->productDescription;
        productDescription = temp;
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
    virtual void print() const = 0;
    virtual const double paidSum() const = 0;
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
    CargoCarPolicy(const CargoCarPolicy &other) : Policy(other)
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
        char *temp = newBrand ? new char[strlen(newBrand) + 1] : nullptr;
        if (temp != nullptr)
            strcpy(temp, newBrand);
        delete[] this->carBrand;
        this->carBrand = temp;
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
        char *temp = newPathway ? new char[strlen(newPathway) + 1] : nullptr;
        if (newPathway != nullptr)
            strcpy(temp, newPathway);
        delete[] this->path;
        path = temp;
    }

    // Virtual functions
    void print() const override
    {
        // std::cout << "Specific data for policy" << std::endl;
    }
    double const paidSum() const override
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

public:
    // No dyn mem ==> no ro3
    // Default constructor
    PropertyFirePolicy() : Policy(), propertyCost(0), area(0), yearBuild(0), hasFireSystem(false) {}
    // Par constructor
    PropertyFirePolicy(const double propertyCostPar, const double areaPar, const int yearBuildPar, const bool hasFireSystemPar) : propertyCost(propertyCostPar), area(areaPar), yearBuild(yearBuildPar), hasFireSystem(hasFireSystemPar) {}

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
    void print() const
    {
    }
    const double paidSum() const override
    {
        // if else chain
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

public:
    // DYN MEM ==> BIG 5
    // Default constructor
    ElectronicEquipmentPolicy() : Policy(), technologyType(nullptr), yearBuild(0), technoValue(0) {}
    // Par constructor
    // ElectronicEquipmentPolicy () {}

    // Virtual functions
    const double paidSum() const override
    {
        return technoValue;
    }
    ElectronicEquipmentPolicy *clone() const override
    {
        return new ElectronicEquipmentPolicy(*this);
    }
    void print() const override
    {
    }
};
