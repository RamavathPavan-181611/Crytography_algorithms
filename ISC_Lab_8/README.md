# DES Round Function f(R, K)

**Course:** Information Security & Cryptography (AI331)  
**Lab 8: DES Round Function Implementation**

## 🎯 Objective

Implement and test the DES round function `f(R, K)` using:

- E-expansion (32 → 48 bits)
- XOR with 48-bit subkey
- 8 S-box substitutions (6 → 4 bits)
- P-permutation (32 → 32 bits)

## 🧩 Files

| File             | Description                                       |
| ---------------- | ------------------------------------------------- |
| `des.hpp`        | Header declaring `feistel()` and `key_schedule()` |
| `des.cpp`        | Implementation of DES round function `f(R,K)`     |
| `des_tables.cpp` | Contains E, P, and S-box tables                   |
| `main.cpp`       | Test harness                                      |
| `README.md`      | This file                                         |

## 🧠 Algorithm Steps

1. **Expand** 32-bit R using E-expansion table.
2. **XOR** the expanded result with the 48-bit subkey.
3. **Substitute** through 8 S-boxes (each 6→4 bits).
4. **Permute** using P-permutation table.
5. **Return** the 32-bit output.

## 🧪 Test Case

**Input:**

clang++ -std=c++17 -c des.cpp
clang++ -std=c++17 -c main.cpp
clang++ des.o main.o -o des

./des

output:
f(R,K) = 0x234AA9BB
