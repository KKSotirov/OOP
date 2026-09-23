#include "Bond.h"
#include <iostream>
#include <cstring>

void Bond::free()
{
    delete[] issuer;
    issuer = nullptr;
}

Bond::Bond() : Asset(), nominalValue(0.0), couponRate(0.0), issuer(nullptr) {}

Bond::Bond(const char *_ticker, const double _nominalValue, const double _couponRate, const char *_issuer) : Asset(_ticker), nominalValue(_nominalValue), couponRate(_couponRate), issuer(nullptr)
{
    setIssuer(_issuer);
}

Bond::Bond(const Bond &other) : Asset(other), nominalValue(other.nominalValue), couponRate(other.couponRate)
{
    setIssuer(other.issuer);
}

Bond::Bond(Bond &&other) noexcept : Asset(std::move(other)), nominalValue(other.nominalValue), couponRate(other.couponRate), issuer(other.issuer)
{
    other.issuer = nullptr;
    other.nominalValue = 0.0;
    other.couponRate = 0.0;
}

Bond &Bond::operator=(const Bond &other)
{
    if (this != &other)
    {
        this->Asset::operator=(other);
        setIssuer(other.issuer);
        setNominalValue(other.nominalValue);
        setCouponRate(other.couponRate);
    }
    return *this;
}

Bond &Bond::operator=(Bond &&other) noexcept
{
    if (this != &other)
    {
        this->Asset::operator=(std::move(other));
        free();

        setNominalValue(other.nominalValue);
        setCouponRate(other.couponRate);
        issuer = other.issuer;

        other.issuer = nullptr;
        other.setNominalValue(0.0);
        other.setCouponRate(0.0);
    }
    return *this;
}

Bond::~Bond()
{
    free();
}

double Bond::getNominalValue() const
{
    return nominalValue;
}

void Bond::setNominalValue(const double _nominalValue)
{
    nominalValue = _nominalValue;
}

double Bond::getCouponRate() const
{
    return couponRate;
}

void Bond::setCouponRate(const double _couponRate)
{
    couponRate = _couponRate;
}

const char *Bond::getIssuer() const
{
    return issuer;
}

void Bond::setIssuer(const char *_issuer)
{
    setterHelper(issuer, _issuer);
}

// Virtual functions
double Bond::getMarketValue() const
{
    return nominalValue * (1.0 + couponRate);
}

void Bond::printInfo() const
{
    std::cout << "[" << this->getTicker()
              << "] Issuer: " << issuer
              << ", Nominal: $" << nominalValue
              << ", Coupon: " << couponRate << "%"
              << " | Total Value: $" << getMarketValue() << "\n";
}

Bond *Bond::clone() const
{
    return new Bond(*this);
}
