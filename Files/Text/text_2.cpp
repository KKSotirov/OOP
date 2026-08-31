#include <iostream>
#include <fstream>
#include <cstring>

int main()
{
    std::ifstream in("text.txt");
    if (!in)
    {
        std::cerr << "Error opening the input file! \n";
        return 1;
    }

    std::ofstream out("long_lines.txt");
    if (!out)
    {
        std::cerr << "Error opening/creating the output file \n";
        in.close();
        return 1;
    }

    int countRows = 0;
    int countChars = 0;
    char line[1024];

    while (in.getline(line, sizeof(line)))
    {
        countRows++;
        int currLineLen = strlen(line);
        countChars += strlen(line);

        if (currLineLen > 20)
        {
            out << line << "\n";
        }
    }

    std::cout << "Count rows = " << countRows << "\n";
    std::cout << "Count characters = " << countChars << "\n";

    in.close();
    out.close();
    return 0;
}
