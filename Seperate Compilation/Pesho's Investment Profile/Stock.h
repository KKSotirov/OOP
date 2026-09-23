#pragma once
#include <Asset.h>

class Stock : public Asset
{
private:
    double pricePerShare;
    int quantity;

public:
    Stock(const char *_ticker, const double _pricePerShare, const int _quantity);

    // Getters + Setters
    double getPricePerShare() const;
    void setPricePerShare(const double _pricePerShare);
    int getQuantity() const;
    void setQuantity(const int _quantity);

    // Virtual function override
    virtual double getMarketValue() const override;
    virtual void printInfo() const override;
    virtual Stock *clone() const override;
};