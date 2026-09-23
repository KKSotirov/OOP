#pragma once
#include <Stock.h>

class Bond : public Asset
{
private:
    double nominalValue;
    double couponRate;
    char *issuer;

    void free();

public:
    Bond();
    Bond(const char *_ticker, const double _nominalValue, const double _couponRate, const char *_issuer);
    Bond(const Bond &other);
    Bond(Bond &&other);
    Bond &operator=(const Bond &other);
    Bond &operator=(Bond &&other);
    ~Bond() override;

    // Getters + Setters
    double getNominalValue() const;
    void setNominalValue(const double _nominalValue);
    double getCouponRate() const;
    void setCouponRate(const double _couponRate);
    const char *getIssuer() const;
    void setIssuer(const char *_issuer);

    // Virtual functions
    virtual double getMarketValue() const override;
    virtual void printInfo() const override;
    virtual Bond *clone() const override;
};