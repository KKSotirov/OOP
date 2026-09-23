#include "Stock.h"
#include <iostream>

Stock::Stock(const char *_ticker, const double _pricePerShare, const int _quantity) : Asset(_ticker), pricePerShare(_pricePerShare), quantity(_quantity) {}

// Getters + Setters
double Stock::getPricePerShare() const
{
    return pricePerShare;
}

void Stock::setPricePerShare(const double _pricePerShare)
{
    pricePerShare = _pricePerShare;
}

int Stock::getQuantity() const
{
    return quantity;
}

void Stock::setQuantity(const int _quantity)
{
    quantity = _quantity;
}

// Virtual functions
double Stock::getMarketValue() const
{
    return pricePerShare * quantity;
}

void Stock::printInfo() const
{
    std::cout << "[" << Stock::getTicker()
              << "] , " << quantity
              << " shares,  market value = "
              << getMarketValue()
              << "\n";
}

Stock *Stock::clone() const
{
    return new Stock(*this);
}
