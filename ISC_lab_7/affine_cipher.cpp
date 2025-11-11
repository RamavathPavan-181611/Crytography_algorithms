// affine_fixed.cpp
// Build: g++ -O2 -std=c++17 affine_fixed.cpp -o affine

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cctype>
#include <cstdlib>
#include <numeric> // For std::gcd (C++17 and later)

using namespace std;

static int mygcd(int a, int b)
{
    while (b != 0)
    {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// ----- small helpers -----
static inline int mod(int x, int m)
{
    int r = x % m;
    return (r < 0) ? r + m : r;
}
static inline bool is_valid_a(int a)
{
    // Valid a iff gcd(a, 26) == 1
    int x = abs(a), y = 26;
    while (y)
    {
        int t = x % y;
        x = y;
        y = t;
    }
    return x == 1;
}

// Convert to uppercase A..Z only , drop everything else
static string sanitize_letters_only_upper(const string &s)
{
    string t;
    t.reserve(s.size());
    for (char c : s)
        if (isalpha((unsigned char)c))
            t.push_back((char)toupper(c));
    return t;
}

// ----- modular inverse a^{ -1} (mod m) via extended Euclid -----
static int modinv(int a, int m)
{
    a = mod(a, m);
    int old_r = a, r = m;
    int old_s = 1, s = 0;

    while (r != 0)
    {
        int q = old_r / r;
        int tmp = old_r - q * r;
        old_r = r;
        r = tmp;

        tmp = old_s - q * s;
        old_s = s;
        s = tmp;
    }
    if (old_r != 1)
        throw runtime_error("modinv: no inverse (gcd!=1)");
    return mod(old_s, m);
}

// ----- encrypt -----
static string encrypt_text(const string &plain, int a, int b)
{
    string res;
    res.reserve(plain.size());
    for (char c : plain)
    {
        int x = c - 'A';
        int y = mod(a * x + b, 26);
        res.push_back('A' + y);
    }
    return res;
}

// ----- decrypt -----
static string decrypt_text(const string &cipher, int a, int b)
{
    int a_inv = modinv(a, 26);
    string res;
    res.reserve(cipher.size());
    for (char c : cipher)
    {
        int y = c - 'A';
        int x = mod(a_inv * (y - b), 26);
        res.push_back('A' + x);
    }
    return res;
}

// === Recover (a,b) from two plaintext-ciphertext pairs ===
// Inputs: x1,y1,x2,y2 are all in 0..25 (A=0, B=1,...Z=25)
// Returns: pair<int,int> (a,b)
static pair<int, int> recover_key_from_two_pairs(int x1, int y1, int x2, int y2)
{
    int m = 26;
    // Compute differences
    int dx = mod(x1 - x2, m);
    int dy = mod(y1 - y2, m);

    if (dx == 0 || mygcd(dx, m) != 1)
        throw runtime_error("Cannot recover key: (x1 - x2) is not invertible modulo 26.");

    int inv_dx = modinv(dx, m); // extended Euclid
    // Solve for a
    int a = mod(dy * inv_dx, m);
    // Solve for b = y1 - a*x1
    int b = mod(y1 - a * x1, m);
    return {a, b};
}

static void brute_force_attack(const string &cipher)
{
    cout << "=== Brute-force attack results ===\n";
    for (int a : {1, 3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25})
    {
        for (int b = 0; b < 26; ++b)
        {
            try
            {
                string plain = decrypt_text(cipher, a, b);
                cout << "a=" << a << " b=" << b << " : " << plain << "\n";
            }
            catch (...)
            {
                // skip any non-invertible a (shouldn’t happen with our list)
            }
        }
    }
}

int main(int argc, char **argv)
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (argc < 2)
    {
        cerr << "Usage:\n"
             << "  " << argv[0] << " enc a b\n"
             << "  " << argv[0] << " dec a b\n"
             << "  " << argv[0] << " recover x1 y1 x2 y2\n"
             << "  " << argv[0] << " attack <CIPHERTEXT>\n";
        return 1;
    }

    string mode = argv[1];

    try
    {
        if (mode == "enc" || mode == "dec")
        {
            if (argc != 4)
            {
                cerr << "Usage: " << argv[0] << " " << mode << " a b\n";
                return 1;
            }
            int a = stoi(argv[2]);
            int b = stoi(argv[3]);

            if (!is_valid_a(a))
            {
                cerr << "Error: a must be coprime with 26.\n";
                return 1;
            }
            b = mod(b, 26);

            // read stdin
            ostringstream oss;
            oss << cin.rdbuf();
            string raw = oss.str();
            string in = sanitize_letters_only_upper(raw);

            if (mode == "enc")
                cout << encrypt_text(in, a, b) << "\n";
            else
                cout << decrypt_text(in, a, b) << "\n";
        }
        else if (mode == "recover")
        {
            if (argc != 6)
            {
                cerr << "Usage: " << argv[0] << " recover x1 y1 x2 y2\n";
                return 1;
            }
            int x1 = stoi(argv[2]), y1 = stoi(argv[3]);
            int x2 = stoi(argv[4]), y2 = stoi(argv[5]);
            auto [rec_a, rec_b] = recover_key_from_two_pairs(x1, y1, x2, y2);
            cout << "Recovered key: a=" << rec_a << " b=" << rec_b << "\n";
        }
        else if (mode == "attack")
        {
            if (argc != 3)
            {
                cerr << "Usage: " << argv[0] << " attack <CIPHERTEXT>\n";
                return 1;
            }
            string cipher = sanitize_letters_only_upper(argv[2]);
            brute_force_attack(cipher);
        }
        else
        {
            cerr << "Mode must be 'enc', 'dec', 'recover', or 'attack'.\n";
            return 1;
        }
    }
    catch (const exception &e)
    {
        cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
