#include <iostream> //OTP -> one time pad
#include <fstream>
#include <vector>
#include <string>
#include <iterator> //
#include <random>   //
#include <cstdlib>  //
using namespace std;

// Read an entire file into a vector of bytes
vector<unsigned char> readFile(const string &filename)
{
    ifstream file(filename, ios::binary);
    if (!file)
    {
        cerr << "Error: Cannot open file " << filename << endl;
        exit(1);
    }
    return vector<unsigned char>((istreambuf_iterator<char>(file)), {});
    // istreambuf_iterator - read the file byte by byte until the end
    //{} - until the end
}

// Write a vector of bytes into a file
void writeFile(const string &filename, const vector<unsigned char> &data)
{
    ofstream file(filename, ios::binary);
    if (!file)
    {
        cerr << "Error: Cannot write to file " << filename << endl;
        exit(1);
    }
    file.write((char *)data.data(), data.size());
}

// Generate a random key of given length
vector<unsigned char> generateKey(size_t length)
{
    vector<unsigned char> key(length);
    random_device rd; // better randomness than rand()
    for (size_t i = 0; i < length; i++)
    {
        key[i] = static_cast<unsigned char>(rd() % 256);
    }
    return key;
}

// XOR operation for encryption/decryption
vector<unsigned char> xorData(const vector<unsigned char> &a,
                              const vector<unsigned char> &b)
{
    if (a.size() != b.size())
    {
        cerr << "Error: Input sizes must match for XOR!" << endl;
        exit(1);
    }
    vector<unsigned char> result(a.size());
    for (size_t i = 0; i < a.size(); i++)
    {
        result[i] = a[i] ^ b[i];
    }
    return result;
}

int main()
{
    // --- Task 1: OTP Encryption & Decryption ---
    auto plaintext = readFile("plaintext.txt");

    // Generate random key of same length
    auto key = generateKey(plaintext.size());
    writeFile("key.bin", key);

    // Encrypt
    auto ciphertext = xorData(plaintext, key);
    writeFile("ciphertext.bin", ciphertext);

    // Decrypt
    auto readKey = readFile("key.bin");
    auto decrypted = xorData(ciphertext, readKey);
    writeFile("decrypted.txt", decrypted);

    cout << "Encryption and Decryption done.\n";

    // --- Task 2: Key Reuse Attack Demo ---
    string M1 = "HELLO";
    string M2 = "WORLD";

    // Convert strings into byte vectors
    vector<unsigned char> m1(M1.begin(), M1.end());
    vector<unsigned char> m2(M2.begin(), M2.end());

    // Use same key for both (bad practice!)
    auto badKey = generateKey(m1.size()); // same length as messages
    auto C1 = xorData(m1, badKey);
    auto C2 = xorData(m2, badKey);

    // Attacker computes C1 ⊕ C2 = M1 ⊕ M2
    auto leak = xorData(C1, C2);

    cout << "\n--- Key Reuse Attack Demo ---\n";
    cout << "M1 = " << M1 << "\n";
    cout << "M2 = " << M2 << "\n";
    cout << "C1 XOR C2 (leak) = M1 XOR M2 = ";

    for (unsigned char c : leak)
        cout << std::hex << std::uppercase << (int)c << " ";
    cout << "\n";

    return 0;
}
