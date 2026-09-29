#include "Asset.h"
#include "Bond.h"
#include "Stock.h"
#include "Portfolio.h"
#include <iostream>

bool isHighValueStock(const Asset *asset)
{
    const Stock *s = dynamic_cast<const Stock *>(asset);
    if (s != nullptr)
        return s->getMarketValue() >= 50000;

    return false;
}

int main()
{
    Portfolio peshos_portfolio;
    Stock nvda("NVDA", 230, 1000);
    Stock aapl("AAPL", 310, 12);
    Bond us10y("US10Y", 1000, 4.5, "US Treasury");
    Bond corp01("CORP01", 5000, 6, "TechCorp");

    peshos_portfolio.addAsset(nvda);
    peshos_portfolio.addAsset(aapl);
    peshos_portfolio.addAsset(us10y);
    peshos_portfolio.addAsset(corp01);

    double portfolio_value = peshos_portfolio.getTotalValue();
    std::cout << "Pesho's portfolio is currently worth: " << portfolio_value << "$\n";

    if (isHighValueStock(&nvda))
        std::cout << "Pesho's Nvidia is a high value stock!\n";
    else
        std::cout << "Pesho's Nvidia is NOT a high value stock\n";

    return 0;
}