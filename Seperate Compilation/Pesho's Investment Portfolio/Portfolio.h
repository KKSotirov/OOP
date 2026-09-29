#pragma once
#include "Asset.h"
#include <iostream>

class Portfolio
{
private:
    Asset **assets;
    size_t count;
    size_t capacity;

    void resize();
    void tighten(const size_t index);
    void free();

public:
    // RO3
    Portfolio();
    Portfolio(const size_t _count);
    Portfolio(const Portfolio &other);
    Portfolio &operator=(const Portfolio &other);
    Portfolio(Portfolio &&other) noexcept;
    Portfolio &operator=(Portfolio &&other) noexcept;
    ~Portfolio();

    void addAsset(const Asset &asset);
    void addAsset(Asset *asset);
    double getTotalValue() const;
    void executeAction(void (*action)(Asset *));
    Portfolio filter(bool (*predicate)(const Asset *)) const;
};