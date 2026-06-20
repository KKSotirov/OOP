const int MAX_ARRAY_SIZE = 50;
#include <iostream>
#include <cstring>
// Задача 4 — Rule of Three
// Напиши клас DynamicArray който пази динамичен масив от int-ове:
// cppclass DynamicArray {
// private:
//     int* data;
//     int size;
// };
// Класът трябва да има:

// Конструктор приемащ размер — заделя паметта
// Копиращ конструктор — прави deep copy
// operator= — прави deep copy
// Деструктор — освобождава паметта
// Метод get(int index) const — връща елемент по индекс
// Метод set(int index, int value) — задава елемент по индекс
// Метод getSize() const — връща размера

class DynamicArray
{
    int *data;
    int size;

public:
    DynamicArray(const int *data, int size) : data(nullptr), size(0)
    {
        this->size = size;
        this->data = new int[size + 1];
    }

    DynamicArray &operator=(const DynamicArray &other)
    {
        if (this != &other)
        {
            delete[] data;
            data = new int[other.size + 1];
            for (int i = 0; i < size; i++)
                this->data[i] = other.data[i];
            size = other.size;
        }
        return *this;
    }

    ~DynamicArray()
    {
        delete[] data;
        data = nullptr;
    }

    // Конструктор приемащ размер — заделя паметта
    // Копиращ конструктор — прави deep copy
    // operator= — прави deep copy
    // Деструктор — освобождава паметта
    // Метод get(int index) const — връща елемент по индекс
    // Метод set(int index, int value) — задава елемент по индекс
    // Метод getSize() const — връща размера

    int get(const int index) const
    {
        return this->data[index];
    }

    void set(const int index, const int value)
    {
        this->data[index] = value;
    }

    int getSize() const
    {
        return this->size;
    }
};

int main()
{

    return 0;
}