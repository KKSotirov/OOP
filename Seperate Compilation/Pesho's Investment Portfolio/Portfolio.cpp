#include <iostream>
#include <cstring>
#include "Portfolio.h"

void Portfolio::resize()
{
    capacity *= 2;
    Asset **tmp = new Asset *[capacity];
    for (size_t i = 0; i < count; i++)
    {
        tmp[i] = assets[i];
    }
    delete[] assets;
    assets = tmp;
}

void Portfolio::tighten(const size_t index)
{
    for (size_t i = index; i < count - 1; i++)
    {
        assets[i] = assets[i + 1];
    }
    count--;
}

void Portfolio::free()
{
    for (size_t i = 0; i < count; i++)
    {
        delete assets[i];
    }
    delete[] assets;
    assets = nullptr;
    count = 0;
    capacity = 0;
}

Portfolio::Portfolio() : capacity(2), count(0)
{
    assets = new Asset *[capacity];
}

Portfolio::Portfolio(const size_t _count) : count(_count), capacity(_count * 2)
{
    assets = new Asset *[capacity];
}

Portfolio::Portfolio(const Portfolio &other) : count(other.count), capacity(other.capacity)
{
    assets = new Asset *[capacity];
    for (size_t i = 0; i < count; i++)
    {
        assets[i] = other.assets[i]->clone();
    }
}

Portfolio &Portfolio::operator=(const Portfolio &other)
{
    if (this != &other)
    {
        free();
        // // VAR 1
        // count = other.count;
        // capacity = other.capacity;
        // for (size_t i = 0; i < count; i++)
        // {
        //     assets[i] = other.assets[i]->clone();
        // }

        // OR VAR 2
        for (size_t i = 0; i < other.count; i++)
            addAsset(*other.assets[i]);
    }
    return *this;
}

Portfolio::Portfolio(Portfolio &&other) noexcept
    : assets(other.assets), count(other.count), capacity(other.capacity)
{
    other.assets = nullptr;
    other.count = 0;
    other.capacity = 0;
}

Portfolio &Portfolio::operator=(Portfolio &&other) noexcept
{
    if (this != &other)
    {
        free();
        assets = other.assets;
        count = other.count;
        capacity = other.capacity;

        other.assets = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
    return *this;
}

Portfolio::~Portfolio()
{
    free();
}

void Portfolio::addAsset(const Asset &asset)
{
    if (count >= capacity)
        resize();
    assets[count++] = asset.clone();
}

void Portfolio::addAsset(Asset *asset)
{
    if (count >= capacity)
        resize();
    assets[count++] = asset;
}

double Portfolio::getTotalValue() const
{
    double value = 0;
    for (size_t i = 0; i < count; i++)
    {
        value += assets[i]->getMarketValue();
    }
    return value;
}

void Portfolio::executeAction(void (*action)(Asset *))
{
    if (action == nullptr)
        return;

    for (size_t i = 0; i < count; ++i)
        action(assets[i]);
}

Portfolio Portfolio::filter(bool (*predicate)(const Asset *)) const
{
    Portfolio resultPortfolio;

    for (size_t i = 0; i < count; ++i)
    {
        if (predicate != nullptr && predicate(assets[i]))
            resultPortfolio.addAsset(assets[i]);
    }

    return resultPortfolio;
}