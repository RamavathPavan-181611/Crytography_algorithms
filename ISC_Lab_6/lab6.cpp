#include <iostream>
#include <vector>
using namespace std;

int modInverse(int a, int m) // brute force
{
    a = (a % m + m) % m; // to ensure the value [0, m-1]
    for (int x = 1; x < m; x++)
    {
        if ((a * x) % m == 1) // modular multiplicative inverse
            return x;
    }
    return -1; // no inverse
}

// Encrypt Block
vector<int> encryptBlock(vector<int> P, vector<vector<int>> K, int mod)
{
    int n = P.size();
    vector<int> C(n, 0);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            C[i] += K[i][j] * P[j];
        }
        C[i] %= mod;
    }
    return C;
}
int determinant(vector<vector<int>> K, int mod)
{
    int n = K.size();
    int det = 0;
    if (n == 2)
    {
        det = K[0][0] * K[1][1] - K[0][1] * K[1][0];
    }
    else if (n == 3)
    {
        det = K[0][0] * (K[1][1] * K[2][2] - K[1][2] * K[2][1]) - K[0][1] * (K[1][0] * K[2][2] - K[1][2] * K[2][0]) + K[0][2] * (K[1][0] * K[2][1] - K[1][1] * K[2][0]);
    }
    det %= mod;
    if (det < 0)
        det += mod;
    return det;
}

// cofactor for 3 x 3
int minorDet(vector<vector<int>> K, int row, int col)
{
    vector<int> vals;
    for (int i = 0; i < 3; i++)
    {
        if (i == row)
            continue; // diagonal element
        for (int j = 0; j < 3; j++)
        {
            if (j == col)
                continue;
            vals.push_back(K[i][j]);
        }
    }
    return vals[0] * vals[3] - vals[1] * vals[2];
}
//[a, b, c]
//[d, e, f]
//[g, h, i]

// Adjoint
vector<vector<int>> adjoint(vector<vector<int>> K)
{
    int n = K.size();
    vector<vector<int>> adj(n, vector<int>(n)); // matrix of n * n
    if (n == 2)
    {
        adj[0][0] = K[1][1];
        adj[0][1] = -K[0][1];
        adj[1][0] = -K[1][0];
        adj[1][1] = K[0][0];
    }
    else if (n == 3)
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                int sign = ((i + j) % 2 == 0 ? 1 : -1);
                // filling row by row
                adj[j][i] = sign * minorDet(K, i, j);
            }
        }
    }
    return adj;
}

// Modular matrix inverse
vector<vector<int>> matrixInverse(vector<vector<int>> K, int mod)
{
    int n = K.size();
    int det = determinant(K, mod);
    int detInv = modInverse(det, mod);

    if (detInv == -1)
    {
        throw runtime_error("Matrix is not invertible modulo 26");
    }

    vector<vector<int>> adj = adjoint(K);
    vector<vector<int>> inv(n, vector<int>(n));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int val = (adj[i][j] * detInv) % mod;
            if (val < 0)
                val += mod;
            inv[i][j] = val;
        }
    }
    return inv;
}
// Decrypt Block
vector<int> decryptBlock(vector<int> C, vector<vector<int>> Kinv, int mod)
{
    int n = C.size();
    vector<int> P(n, 0);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            P[i] += Kinv[i][j] * C[j];
        }
        P[i] %= mod;
    }
    return P;
}

int main()
{
    // int a = 1;
    // int m = 2;
    // cout << (a % m + m) % m; //1
    // return 0;

    string plaintext = "OVERSHADOW";
    vector<vector<int>> K2 = {{3, 3, 3}, {2, 5, 5}};
    int mod = 26;
    int n = K2.size();

    // convert plaintext into numbers
    vector<int> nums;
    for (char c : plaintext)
        nums.push_back(c - 'A');

    // pad if needed
    while (nums.size() % n != 0)
        nums.push_back('X' - 'A');
    // HONDA nums.size = 5 % 2 we can't divide equal parts
    // so we add filler(X) then size becomes 6 % 2 == 0 (required)

    // Encryption
    string ciphertext = "";
    for (int i = 0; i < nums.size(); i += n)
    {
        // dividing blocks of size n(2,3)
        vector<int> block(nums.begin() + i, nums.begin() + i + n);
        vector<int> enc = encryptBlock(block, K2, mod);
        for (int x : enc)
            ciphertext += (char)(x + 'A');
    }
    // Decryption
    vector<vector<int>> Kinv = matrixInverse(K2, mod);
    string recovered = "";
    for (int i = 0; i < nums.size(); i += n) // error
    {
        vector<int> block;
        for (int j = 0; j < n; j++)
            block.push_back(ciphertext[i + j] - 'A'); // convert back int
        vector<int> dec = decryptBlock(block, Kinv, mod);
        for (int x : dec)
            recovered += (char)(x + 'A');
    }
    cout << "Plaintext:  " << plaintext << endl;
    cout << "Ciphertext: " << ciphertext << endl;
    cout << "Decrypted:  " << recovered << endl;
    return 0;
}