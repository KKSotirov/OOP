#include "Asset.h"
#include <iostream>
#include <cstring>

void Asset::free()
{
    delete[] ticker;
}

void Asset::setterHelper(char *&dest, const char *src)
{
    char *tmp = src ? new char[strlen(src) + 1] : nullptr;
    if (src)
        strcpy(tmp, src);
    delete[] dest;
    dest = tmp;
}

Asset::Asset() : ticker(nullptr) {}

Asset::Asset(const char *_ticker) : ticker(nullptr)
{
    setTicker(_ticker);
}

Asset::Asset(const Asset &other) : ticker(nullptr)
{
    setTicker(other.ticker);
}

Asset &Asset::operator=(const Asset &other)
{
    if (this != &other)
        setTicker(other.ticker);

    return *this;
}

Asset::Asset(Asset &&other) noexcept : ticker(other.ticker)
{
    other.ticker = nullptr;
}

Asset &Asset::operator=(Asset &&other) noexcept
{
    if (this != &other)
    {
        free();
        ticker = other.ticker;
        other.ticker = nullptr;
    }
    return *this;
}

Asset::~Asset()
{
    free();
}

// Getters + Setters
const char *Asset::getTicker() const
{
    return ticker;
}

void Asset::setTicker(const char *_ticker)
{
    setterHelper(ticker, _ticker);
}
