// sha1.cpp
// Simple SHA-1 implementation for educational purposes
// Compile: g++ -std=c++17 sha1.cpp -o sha1

#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

using u8 = uint8_t;
using u32 = uint32_t;
using u64 = uint64_t;

// left rotate 32-bit
static inline u32 rol(u32 x, unsigned s)
{
    return (x << s) | (x >> (32 - s));
}

// Convert a string to a vector of bytes (ASCII)
vector<u8> string_to_bytes(const string &s)
{
    return vector<u8>(s.begin(), s.end());
}

// Append padding bits and length to produce message multiple of 512 bits.
// Also produces a string of bits for display when requested.
vector<u8> sha1_pad(const vector<u8> &msg, string *out_padded_bits = nullptr)
{
    vector<u8> m = msg;                // copy
    u64 orig_bits = (u64)m.size() * 8; // length in bits

    // append 1 bit (0x80) then zero bytes until length ≡ 448 (mod 512)
    m.push_back(0x80);

    // append 0x00 bytes until message length in bits ≡ 448 (mod 512)
    while (((m.size() * 8) % 512) != 448)
    {
        m.push_back(0x00);
    }

    // append original length as 64-bit big-endian
    for (int i = 7; i >= 0; --i)
    {
        m.push_back(static_cast<u8>((orig_bits >> (i * 8)) & 0xFF));
    }

    // optional: produce padded binary string for display
    if (out_padded_bits)
    {
        string bits;
        bits.reserve(m.size() * 8);
        for (u8 b : m)
        {
            for (int i = 7; i >= 0; --i)
                bits.push_back(((b >> i) & 1) ? '1' : '0');
            bits.push_back(' '); // separate bytes (optional)
        }
        *out_padded_bits = bits;
    }

    return m;
}

// Process message in 512-bit blocks and compute SHA-1 digest
array<u32, 5> sha1_hash(const vector<u8> &padded, bool print_intermediate = false)
{
    // initial H values (hex)
    u32 H0 = 0x67452301;
    u32 H1 = 0xEFCDAB89;
    u32 H2 = 0x98BADCFE;
    u32 H3 = 0x10325476;
    u32 H4 = 0xC3D2E1F0;

    size_t nblocks = padded.size() * 8 / 512;
    for (size_t block = 0; block < nblocks; ++block)
    {
        // Prepare message schedule W[0..79]
        u32 W[80] = {0};

        // each block has 16 big-endian 32-bit words
        const u8 *block_ptr = padded.data() + block * 64; // 64 bytes per block
        for (int t = 0; t < 16; ++t)
        {
            W[t] = (u32(block_ptr[t * 4 + 0]) << 24) | (u32(block_ptr[t * 4 + 1]) << 16) | (u32(block_ptr[t * 4 + 2]) << 8) | (u32(block_ptr[t * 4 + 3]) << 0);
        }
        // extend words
        for (int t = 16; t < 80; ++t)
        {
            W[t] = rol(W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16], 1);
        }

        // Initialize working vars
        u32 A = H0, B = H1, C = H2, D = H3, E = H4;

        // Main loop
        for (int t = 0; t < 80; ++t)
        {
            u32 f, K;
            if (t <= 19)
            {
                f = (B & C) | ((~B) & D);
                K = 0x5A827999;
            }
            else if (t <= 39)
            {
                f = B ^ C ^ D;
                K = 0x6ED9EBA1;
            }
            else if (t <= 59)
            {
                f = (B & C) | (B & D) | (C & D);
                K = 0x8F1BBCDC;
            }
            else
            {
                f = B ^ C ^ D;
                K = 0xCA62C1D6;
            }

            u32 temp = rol(A, 5) + f + E + K + W[t];
            E = D;
            D = C;
            C = rol(B, 30);
            B = A;
            A = temp;
        }

        // Add this block's hash to result so far
        H0 = H0 + A;
        H1 = H1 + B;
        H2 = H2 + C;
        H3 = H3 + D;
        H4 = H4 + E;

        if (print_intermediate)
        {
            auto hex32 = [](u32 x)
            {
                ostringstream ss;
                ss << hex << setw(8) << setfill('0') << (uint32_t)x;
                return ss.str();
            };
            cout << "After block " << block << " intermediate H: "
                 << hex32(H0) << " " << hex32(H1) << " "
                 << hex32(H2) << " " << hex32(H3) << " "
                 << hex32(H4) << "\n";
        }
    }

    return {H0, H1, H2, H3, H4};
}

// Helper to produce hex digest string
string digest_to_hex(const array<u32, 5> &H)
{
    ostringstream ss;
    ss << hex << nouppercase << setfill('0');
    for (u32 h : H)
    {
        ss << setw(8) << (uint32_t)h;
    }
    return ss.str();
}

int main(int argc, char **argv)
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string input;
    bool read_from_stdin = false;
    bool print_intermediate = false;
    if (argc >= 2)
    {
        // if first argument is "-", read from stdin
        if (string(argv[1]) == "-")
        {
            read_from_stdin = true;
        }
        else
        {
            // join all arguments with spaces as message
            input.clear();
            for (int i = 1; i < argc; ++i)
            {
                if (i > 1)
                    input.push_back(' ');
                input += argv[i];
            }
        }
        // optional flag --debug to print intermediate H
        if (argc >= 3 && string(argv[argc - 1]) == "--debug")
            print_intermediate = true;
    }
    else
    {
        // interactive prompt
        cout << "Enter message (empty line for empty message):\n";
        std::getline(cin, input);
    }

    if (read_from_stdin)
    {
        // read entire stdin into input
        ostringstream ss;
        ss << cin.rdbuf();
        input = ss.str();
        // remove trailing newline if present (depending on use case)
        if (!input.empty() && input.back() == '\n')
            input.pop_back();
    }

    vector<u8> bytes = string_to_bytes(input);

    // produce padded bytes and bitstring for display
    string padded_bits;
    vector<u8> padded = sha1_pad(bytes, &padded_bits);

    cout << "Input message: \"" << input << "\"\n";
    cout << "Message length (bits): " << bytes.size() * 8 << "\n\n";

    cout << "Padded message bits (groups of 8 bits per byte shown with spaces):\n";
    // Print only first 1024 chars of the padded bits to avoid too huge output for long messages
    if (padded_bits.size() > 4096)
    {
        cout << padded_bits.substr(0, 4096) << " ... (truncated)\n\n";
    }
    else
    {
        cout << padded_bits << "\n\n";
    }

    // compute hash
    array<u32, 5> H = sha1_hash(padded, print_intermediate);

    string hex_digest = digest_to_hex(H);
    cout << "SHA-1 digest: " << hex_digest << "\n";

    // For verification, also print lower-case as the expected outputs are lower-case hex
    return 0;
}
