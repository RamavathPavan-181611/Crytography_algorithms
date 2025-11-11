For this lab, using long long is completely fine and much simpler.

If in the future we do real RSA with 1024/2048-bit keys, then you’ll need cpp_int or a big integer library.
for that we have install boost library in homebrew

1. Why e must be coprime with φ(n):
   So that a modular inverse d exists, ensuring correct decryption.

2. Why modular inverse is needed:
   d is computed as the modular inverse of e modulo φ(n); it forms the private key.

3. If m ≥ n:
   RSA encryption works modulo n. If m >= n, it will be reduced modulo n, which can cause loss of information.

4. Why textbook RSA is not secure:
   It’s deterministic and malleable. It lacks padding, making it vulnerable to chosen-plaintext and other attacks. Real systems use padding schemes like OAEP

ouput:
=== RSA Implementation (Toy Example, No Boost) ===
p = 61
q = 53
n = 3233
phi(n) = 3120
Public exponent e = 17
Private exponent d = 2753

Message m = 65
Ciphertext c = 2790
Decrypted message m_dec = 65

[OK] Decryption successful!
