#include <iostream>
#include <fstream>
#include <string>
using namespace std;

char shiftChar(char c, int shift)
{
    if (c >= 'a' && c <= 'z')
    {
        return (((c - 'a' - shift) % 26 + 26) % 26) + 'a'; // adding extra 26 for number becomes -ve
    }
    else if (c >= 'A' && c <= 'Z')
    {
        return (((c - 'A' - shift) % 26 + 26) % 26) + 'A';
    }
    else
    {
        return c; // Leave punctuation, digits, etc. unchanged
    }
}

int main()
{
    ifstream input("output.txt");
    ofstream output("decrypted.txt");

    if (!input.is_open())
    {
        cout << "File not opened properly." << endl;
        return 1;
    }

    int shift = 3;
    string line;

    while (getline(input, line))
    {
        string decryptedLine;
        for (char c : line)
        {
            decryptedLine += shiftChar(c, shift);
        }
        output << decryptedLine << '\n';
    }

    input.close();
    output.close();

    cout << "File is decrypted and saved in decrypted.txt" << endl;

    return 0;
}
