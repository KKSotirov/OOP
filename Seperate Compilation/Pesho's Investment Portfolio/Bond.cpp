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