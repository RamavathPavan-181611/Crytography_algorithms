# Affine Cipher Lab 7 -- README

## Build Instructions

Compile the program using g++:

```bash
g++ -O2 -std=c++17 affine_cipher.cpp -o affine
```

This produces the executable `affine`.

## Run Instructions

### Encryption

Encrypt plaintext from standard input:

```bash
echo HELLOWORLD | ./affine enc 5 8
# Output: RCLLAOAPLX
```

### Decryption

Decrypt ciphertext from standard input:

```bash
echo RCLLAOAPLX | ./affine dec 5 8
# Output: HELLOWORLD
```

### Frequency Attack (Task B2)

Run frequency attack on a ciphertext string:

```bash
./affine attack RCLLAOAPLX
```

This prints the two most frequent cipher letters and candidate `(a,b)`
keys with their decrypted plaintext guesses.

## Text-Handling Policy

- The program keeps **only letters A..Z** (uppercased) and **drops all
  other characters** (spaces, punctuation, digits).
- All input text is converted to uppercase before processing.
- Non-letter characters are not preserved in encryption or decryption.

## Brief Summary of Task B Approach

### B1: Two-Letter Mapping

If two plaintext--ciphertext letter correspondences are known, the
program solves:

    y1 ≡ a x1 + b (mod 26)
    y2 ≡ a x2 + b (mod 26)

Subtracting eliminates `b` and allows recovery of `a` using the modular
inverse of `(x1−x2)` mod 26, then `b = y1 − a x1` mod 26.

### B2: Frequency Heuristic

The program counts the two most frequent letters in the ciphertext and
tries both mappings (E→ymax, T→y2nd) and (E→y2nd, T→ymax). For each
pairing, it recovers `(a,b)` as above and decrypts the ciphertext. The
candidate producing a readable plaintext is chosen.

# recover a,b from the two pairs used in your sample:

./affine recover 7 17 4 2

# -> Recovered key: a=5 b=8
