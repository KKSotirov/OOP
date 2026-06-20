#include <iostream>
const int MAX_STACK_SIZE = 100;
// Задача 5 — Шаблонен стек
// Напиши шаблонен клас Stack<T> с:

// push(T value) — добавя елемент отгоре
// pop() — премахва елемент отгоре
// peek() const — връща горния елемент без да го премахва
// isEmpty() const — проверява дали е празен

// Можеш да ползваш фиксиран масив с максимален размер (например 100).
// решението - typename T-> class with objects of type T, methods
template <typename T>
class Stack
{
private:
    T data[MAX_STACK_SIZE];
    int topIndex = -1;

public:
    void push(T value)
    {
        // nie znam posledniqt index, ne poslednata stoinost XDDDDD
        data[topIndex + 1] = value;
        topIndex += 1;
    }
    void pop()
    {
        // delete data[topIndex];
        topIndex -= 1;
    }
    T peek() const
    {
        return data[topIndex];
    }
    bool isEmpty() const
    {
        if (topIndex < 0)
            return true;
        return false;
    }
};

int main()
{
    return 0;
}