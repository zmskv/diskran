#include <iostream>
#include <vector>
#include <string>
using namespace std;

vector<int> buildZ(const string &s)
{
    int n = s.size();
    vector<int> z(n);
    int l = 0, r = 0;
    for (int i = 1; i < n; ++i)
    {
        if (i <= r)
            z[i] = min(r - i + 1, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]])
            ++z[i];
        if (i + z[i] - 1 > r)
        {
            l = i;
            r = i + z[i] - 1;
        }
    }
    return z;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string text, pattern;
    getline(cin, text);
    getline(cin, pattern);

    string mask = pattern + '#' + text;
    vector<int> z = buildZ(mask);
    int length = pattern.size();

    for (int i = length + 1; i < z.size(); ++i)
    {
        if (z[i] == length)
        {
            cout << (i - length - 1) << '\n';
        }
    }

    return 0;
}
