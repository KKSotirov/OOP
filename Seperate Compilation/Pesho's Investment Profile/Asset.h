#pragma once

class Asset
{
private:
    char *ticker;

    void free();
    void setterHelper(char *&dest, const char *src);

public:
    Asset();
    Asset(const char *_ticker);
    Asset(const Asset &other);
    Asset &operator=(const Asset &other);
    Asset(Asset &&other);
    Asset &operator=(Asset &&other) noexcept;
    virtual ~Asset();

    // Getters + Setters
    const char *getTicker() const;
    void setTicker(const char *_ticker);

    // Virtual functions
    virtual double getMarketValue() const = 0;
    virtual void printInfo() const = 0;
    virtual Asset *clone() const = 0;
};