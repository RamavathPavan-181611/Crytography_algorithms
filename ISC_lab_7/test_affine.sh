#!/bin/bash
# Small test script for affine_cipher.cpp

# Ensure the binary is built
if [ ! -x ./affine ]; then
  echo "Binary ./affine not found. Compile first:"
  echo "  g++ -O2 -std=c++17 affine_cipher.cpp -o affine"
  exit 1
fi

echo "== Affine Cipher Tests =="

# Test 1: encryption
PT="HELLOWORLD"
A=5
B=8
CT=$(echo "$PT" | ./affine enc $A $B)
echo "Encrypt '$PT' (a=$A,b=$B) -> $CT"

# Test 2: decryption
DT=$(echo "$CT" | ./affine dec $A $B)
echo "Decrypt '$CT' (a=$A,b=$B) -> $DT"

# Test 3: second test vector from lab
PT2="CRYPTOGRAPHY"
A2=7
B2=3
CT2=$(echo "$PT2" | ./affine enc $A2 $B2)
echo "Encrypt '$PT2' (a=$A2,b=$B2) -> $CT2"
DT2=$(echo "$CT2" | ./affine dec $A2 $B2)
echo "Decrypt '$CT2' (a=$A2,b=$B2) -> $DT2"

# Test 4: frequency attack demo
echo "Frequency attack on $CT:"
./affine attack $CT


# bash script for running and executing

# chmod +x test_affine.sh

# ./test_affine.sh

