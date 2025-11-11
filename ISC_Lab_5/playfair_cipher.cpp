// U23AI130

#include <iostream>
#include <map>
using namespace std;

// Normalize input: keep A-Z, map J->I
string normalize(const string &s)
{
    string r;
    for (char ch : s)
    {
        if ('a' <= ch && ch <= 'z')
            r.push_back(ch - 'a' + 'A');
        else if ('A' <= ch && ch <= 'Z')
            r.push_back(ch);
    }
    for (char &ch : r)
        if (ch == 'J')
            ch = 'I';
    return r;
}

// --- Build 5x5 key square ---
array<array<char, 5>, 5> buildKeySquare(const string &keyword, map<char, pair<int, int>> &pos)
{
    string key = normalize(keyword);
    string used;
    vector<bool> seen(26, false);

    for (char c : key)
    {
        if (c == 'J')
            c = 'I'; // merge
        if (!seen[c - 'A'])
        {
            used.push_back(c);
            seen[c - 'A'] = true;
        }
    }
    for (char c = 'A'; c <= 'Z'; c++)
    {
        if (c == 'J')
            continue;
        if (!seen[c - 'A'])
        {
            used.push_back(c);
            seen[c - 'A'] = true;
        }
    }

    array<array<char, 5>, 5> grid;
    int idx = 0;
    for (int r = 0; r < 5; r++)
    {
        for (int c = 0; c < 5; c++)
        {
            grid[r][c] = used[idx];
            pos[used[idx]] = {r, c};
            idx++;
        }
    }
    return grid;
}

// --- Convert plaintext into digraphs ---
vector<pair<char, char>> toDigraphs(const string &plaintext)
{
    string s = normalize(plaintext);
    vector<pair<char, char>> res;
    for (int i = 0; i < (int)s.size();)
    {
        char a = s[i];
        char b;
        if (i + 1 < (int)s.size())
        {
            b = s[i + 1];
            if (a == b)
            { // insert X
                b = 'X';
                i++;
            }
            else
            {
                i += 2;
            }
        }
        else
        {
            b = 'X'; // pad
            i++;
        }
        res.push_back({a, b});
    }
    return res;
}

// --- Encrypt digraphs ---
string encrypt(const string &plaintext, const array<array<char, 5>, 5> &grid, map<char, pair<int, int>> &pos)
{
    auto digraphs = toDigraphs(plaintext);
    string res;
    for (auto [a, b] : digraphs)
    {
        auto [ra, ca] = pos[a];
        auto [rb, cb] = pos[b];
        if (ra == rb)
        { // same row
            res.push_back(grid[ra][(ca + 1) % 5]);
            res.push_back(grid[rb][(cb + 1) % 5]);
        }
        else if (ca == cb)
        { // same column
            res.push_back(grid[(ra + 1) % 5][ca]);
            res.push_back(grid[(rb + 1) % 5][cb]);
        }
        else
        { // rectangle
            res.push_back(grid[ra][cb]);
            res.push_back(grid[rb][ca]);
        }
    }
    return res;
}

// --- Decrypt ciphertext ---
string decrypt(const string &ciphertext, const array<array<char, 5>, 5> &grid, map<char, pair<int, int>> &pos)
{
    string s = normalize(ciphertext);
    string res;
    for (int i = 0; i < (int)s.size(); i += 2)
    {
        char a = s[i], b = s[i + 1];
        auto [ra, ca] = pos[a];
        auto [rb, cb] = pos[b];
        if (ra == rb)
        { // same row -> left
            res.push_back(grid[ra][(ca + 4) % 5]);
            res.push_back(grid[rb][(cb + 4) % 5]);
        }
        else if (ca == cb)
        { // same col -> up
            res.push_back(grid[(ra + 4) % 5][ca]);
            res.push_back(grid[(rb + 4) % 5][cb]);
        }
        else
        { // rectangle
            res.push_back(grid[ra][cb]);
            res.push_back(grid[rb][ca]);
        }
    }
    return res;
}

int main(int argc, char **argv)
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string mode, key, text;
    if (argc > 1)
    {
        for (int i = 1; i < argc; ++i)
        {
            string a = argv[i];
            if (a == "--mode" && i + 1 < argc)
                mode = argv[++i];
            else if (a == "--key" && i + 1 < argc)
                key = argv[++i];
            else if (a == "--text" && i + 1 < argc)
                text = argv[++i];
        }
    }
    if (mode.empty())
    {
        if (!getline(cin, mode))
            return 0;
        getline(cin, key);
        getline(cin, text);
    }

    try
    {
        map<char, pair<int, int>> pos;
        auto grid = buildKeySquare(key, pos);
        if (mode == "enc")
        {
            cout << encrypt(text, grid, pos) << "\n";
        }
        else if (mode == "dec")
        {
            cout << decrypt(text, grid, pos) << "\n";
        }
        else
        {
            cerr << "Invalid mode. Use enc|dec.\n";
            return 1;
        }
    }
    catch (const exception &e)
    {
        cerr << "[ERROR] " << e.what() << "\n";
        return 2;
    }
    return 0;
}
