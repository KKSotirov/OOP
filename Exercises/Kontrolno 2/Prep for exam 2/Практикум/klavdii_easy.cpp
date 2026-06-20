#include <iostream>
#include <fstream>

//------------------------1----------------------
// Напиши шаблонна функция swap която разменя стойностите на два елемента от произволен тип.
template <typename T>
void mySwap(T &x, T &y)
{
    T temp = x;
    x = y;
    y = temp;
}
//------------------------2----------------------
// Напиши клас Counter който:
// Пази едно цяло число (започва от 0)
// Има метод increment() който го увеличава с 1
// Има метод reset() който го нулира
// Има метод getValue() който връща стойността — и не трябва да може да се вика върху const Counter... или трябва? Помисли.
class Counter
{
    int currentCount = 0;

public:
    void increment()
    {
        currentCount += 1;
    }
    void reset()
    {
        currentCount = 0;
    }
    int getValue() const
    {
        return currentCount;
    }
};
//------------------------3----------------------
struct Student
{
    char name[50];
    int grade;
};

int main()
{
    Student s1 = {"Gosho", 2};

    std::ofstream file("students.txt");
    if (!file.is_open())
    {
        std::cerr << "File not opened successfuly!" << std::endl;
        return 1;
    }
    else
    {
        file << s1.name << std::endl
             << s1.grade << std::endl;
    }
    file.close();

    std::ifstream read("students.txt");
    if (!read.is_open())
    {
        std::cerr << "File not opened successfuly!" << std::endl;
        return 1;
    }
    else
    {
        char name[50];
        int grade;
        read.getline(name, 50);
        read >> grade;
        std::cout << "Name : " << name << ", grade : " << grade << std::endl;
    }

    read.close();
    return 0;
}