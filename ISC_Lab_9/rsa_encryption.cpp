#include <iostream>
using namespace std;

// ---------- (1) Extended Euclidean Algorithm ----------
struct EG
{
    long long g, x, y;
};

EG egcd(long long a, long long b)
{
    if (b == 0)
        return {a, 1, 0};
    EG t = egcd(b, a % b);
    long long g = t.g;
    long long x = t.y;
    long long y = t.x - (a / b) * t.y;
    return {g, x, y};
}

// ---------- (2) Modular Inverse ----------
long long modinv(long long a, long long m) // e, phi// de = 1 mod phi
{
    EG t = egcd(a, m);
    if (t.g != 1)
    {
        throw runtime_error("Modular inverse does not exist");
    }
    long long inv = t.x % m;
    if (inv < 0)
        inv += m;
    return inv;
}

// ---------- (3) Modular Exponentiation ----------
long long modexp(long long base, long long exp, long long mod)
{
    base %= mod;
    long long res = 1;
    while (exp > 0)
    {
        if (exp & 1)
            res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return res;
}

int main()
{
    cout << "=== RSA Implementation (Toy Example, No Boost) ===\n";

    // ---------- (4) Key Generation ----------
    long long p = 61, q = 53; // Example primes
    long long n = p * q;
    long long phi = (p - 1) * (q - 1);
    long long e = 17; // Common choice

    if (egcd(e, phi).g != 1)
    {
        cerr << "e and phi(n) are not coprime!" << endl;
        return 1;
    }

    long long d = modinv(e, phi);

    cout << "p = " << p << "\nq = " << q << "\n";
    cout << "n = " << n << "\nphi(n) = " << phi << "\n";
    cout << "Public exponent e = " << e << "\nPrivate exponent d = " << d << "\n";

    // ---------- (5) Encrypt & Decrypt ----------
    long long m = 65; // Example message
    if (m >= n)
    {
        cerr << "Message must be less than n!" << endl;
        return 1;
    }

    long long c = modexp(m, e, n);
    long long m_dec = modexp(c, d, n);

    cout << "\nMessage m = " << m;
    cout << "\nCiphertext c = " << c;
    cout << "\nDecrypted message m_dec = " << m_dec << "\n";

    if (m == m_dec)
        cout << "\n[OK] Decryption successful!\n";
    else
        cout << "\n[ERROR] Decryption failed!\n";

    return 0;
}
