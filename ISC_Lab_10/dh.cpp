#include <iostream>
#include <random>
#include <cstdint>

using namespace std;

// ---------- Task 1: Modular Exponentiation ----------
// Compute (base^exp) % mod using repeated squaring (fast exponentiation)
uint64_t modexp(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base = base % mod;

    while (exp > 0)
    {
        if (exp & 1)                                   // if exponent is odd
            result = (__uint128_t)result * base % mod; // use 128-bit to avoid overflow
        base = (__uint128_t)base * base % mod;
        exp >>= 1; // divide exponent by 2
    }
    return result;
}

int main()
{
    // ---------- Task 2: Simulate Alice and Bob ----------
    // Public parameters
    const uint64_t p = 4294967311ULL; // 64-bit prime (demo purpose)
    const uint64_t g = 5ULL;          // generator

    // Random number generator for secrets
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<uint64_t> dist(2, p - 2);

    uint64_t a = dist(gen); // Alice's private key
    uint64_t b = dist(gen); // Bob's private key

    // Compute public keys
    uint64_t A = modexp(g, a, p); // A = g^a mod p
    uint64_t B = modexp(g, b, p); // B = g^b mod p

    // Each computes shared secret
    uint64_t KA = modexp(B, a, p); // Alice computes
    uint64_t KB = modexp(A, b, p); // Bob computes

    // ---------- Task 3: Print All Values ----------
    cout << "Public prime p = " << p << endl;
    cout << "Generator g = " << g << endl;
    cout << "Alice private a = " << a << endl;
    cout << "Alice public A = " << A << endl;
    cout << "Bob private b = " << b << endl;
    cout << "Bob public B = " << B << endl;
    cout << "Shared key computed by Alice = " << KA << endl;
    cout << "Shared key computed by Bob = " << KB << endl;
    cout << "Keys match: " << (KA == KB ? "YES" : "NO") << endl;

    return 0;
}

// how it working:

// (a mod m)^k mod m= (a^k) mod m
// for our case
//(a^b)^c mod m = (a^c)^b mod m= a ^(b⋅c) mod m
// hence we get same shared key