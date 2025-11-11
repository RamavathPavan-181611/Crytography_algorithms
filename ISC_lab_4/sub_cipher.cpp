// Substitution Cipher with File I/O + Frequency Analysis
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
using namespace std;

// Generate random substitution key
string generateKey()
{
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    random_shuffle(alphabet.begin(), alphabet.end());
    return alphabet;
}

// Encrypt function
string encrypt(string text, string key)
{
    string result = "";
    for (char c : text)
    {
        if (isalpha(c))
        {
            bool isLower = islower(c);
            char upper = toupper(c);
            int idx = upper - 'A';
            char mapped = key[idx];
            result += (isLower ? tolower(mapped) : mapped);
        }
        else
        {
            result += c;
        }
    }
    return result;
}

// Decrypt function
string decrypt(string cipher, string key)
{
    string result = "";
    vector<char> invMap(26);
    for (int i = 0; i < 26; i++)
    {
        invMap[key[i] - 'A'] = 'A' + i;
    }

    for (char c : cipher)
    {
        if (isalpha(c))
        {
            bool isLower = islower(c);
            char upper = toupper(c);
            int idx = upper - 'A';
            char mapped = invMap[idx];
            result += (isLower ? tolower(mapped) : mapped);
        }
        else
        {
            result += c;
        }
    }
    return result;
}

// Frequency Analysis
void frequencyAnalysis(string cipher)
{
    vector<int> count(26, 0);
    int total = 0;

    for (char c : cipher)
    {
        if (isalpha(c))
        {
            count[toupper(c) - 'A']++;
            total++;
        }
    }

    cout << "\nCiphertext Frequency Analysis:\n";
    cout << "Letter | Count | Percentage\n";
    cout << "---------------------------\n";
    for (int i = 0; i < 26; i++)
    {
        double percent = (total > 0) ? (count[i] * 100.0 / total) : 0;
        cout << char('A' + i) << "      | "
             << setw(5) << count[i] << " | "
             << fixed << setprecision(2) << percent << "%" << endl;
    }

    // Standard English letter frequencies (approximate)
    vector<double> english = {
        8.2, 1.5, 2.8, 4.3, 13.0, 2.2, 2.0, 6.1, 7.0,
        0.15, 0.77, 4.0, 2.4, 6.7, 7.5, 1.9, 0.095, 6.0,
        6.3, 9.1, 2.8, 0.98, 2.4, 0.15, 2.0, 0.074};

    cout << "\nStandard English Frequencies (%):\n";
    for (int i = 0; i < 26; i++)
    {
        cout << char('A' + i) << ": " << english[i] << "%  ";
        if ((i + 1) % 6 == 0)
            cout << endl;
    }
}

int main()
{
    srand(time(0));
    string key = generateKey();

    cout << "Key mapping:\n";
    for (int i = 0; i < 26; i++)
    {
        cout << char('A' + i) << " -> " << key[i] << endl;
    }

    // Read plaintext from file
    ifstream inFile("plaintext.txt");
    if (!inFile)
    {
        cerr << "Error: Could not open plaintext.txt\n";
        return 1;
    }

    stringstream buffer;
    buffer << inFile.rdbuf(); // read entire file into buffer
    string plaintext = buffer.str();
    inFile.close();

    cout << "\nPlaintext read from plaintext.txt:\n"
         << plaintext << endl;

    // Encrypt
    string cipher = encrypt(plaintext, key);

    // Write ciphertext to file
    ofstream outCipher("ciphertext.txt");
    outCipher << cipher;
    outCipher.close();

    cout << "\nCiphertext written to ciphertext.txt\n";

    // Decrypt
    string decrypted = decrypt(cipher, key);

    // Write decrypted text to file
    ofstream outDec("decrypted.txt");
    outDec << decrypted;
    outDec.close();

    cout << "Decrypted text written to decrypted.txt\n";

    // Perform frequency analysis
    frequencyAnalysis(cipher);

    return 0;
}

// 1 ans) there 26! possible keys and as it impossible to go with 26! keys with brute force
// 2 ans) substitute cipher is theoritically secure but practically in english alphabets e, t, a, o
// most used
// 3 ans) with frequency analysis of letter it become to easy to decrypt cipher text
