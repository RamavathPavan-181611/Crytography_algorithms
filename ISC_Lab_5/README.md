Lab 5 – Playfair Cipher

Build Instruction:

g++ -std=c++17 playfair_cipher.cpp -o playfair

Run Instruction:

Encryption:

./playfair --mode enc --key "MONARCHY" --text "INSTRUMENTS"
./playfair --mode enc --key "PLAYFAIREXAMPLE" --text "HIDE THE GOLD IN THE TREE STUMP"
./playfair --mode enc --key "SECURITY" --text "BALLOON"
./playfair --mode enc --key "CRYPTOGRAPHY" --text "DEFEND THE EAST WALL OF THE CASTLE"
./playfair --mode enc --key "KNOWLEDGE" --text "KICK"

Decryption:

./playfair --mode dec --key "MONARCHY" --text "GATLMZCLRQXA"
./playfair --mode dec --key "PLAYFAIREXAMPLE" --text "BMODZBXDNABEKUDMUIXMMOUVIF"
./playfair --mode dec --key "SECURITY" --text "IBOQMPPO"
./playfair --mode dec --key "CRYPTOGRAPHY" --text "EFIFLFPBIVFGZBYFQUUDKYGIYOZBMD"
./playfair --mode dec --key "KNOWLEDGE" --text "WCPE"
