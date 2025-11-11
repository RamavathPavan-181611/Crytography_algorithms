#include <iostream>
#include <fstream>
#include <string>
using namespace std;

char shiftChar(char c, int shift)
{
    if (c >= 'a' && c <= 'z')
        return ((c - 'a' + shift) % 26) + 'a';
    else if (c >= 'A' && c <= 'Z')
        return ((c - 'A' + shift) % 26) + 'A';
    else
        return c; // return unchanged if non-letter
}

int main()
{
    ifstream input("input.txt");
    ofstream output("output.txt");
    string line;

    if (!input.is_open())
    {
        cout << "File not opened properly." << endl;
        return 1;
    }

    int shift = 3;

    while (getline(input, line))
    {
        string resultLine;
        for (char c : line)
        {
            resultLine += shiftChar(c, shift);
        }
        output << resultLine << '\n';
    }

    input.close();
    output.close();

    cout << "File is encrypted and saved in output.txt" << endl;

    return 0;
}
