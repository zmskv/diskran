#include <iostream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

void countSort(vector<pair<string, string>> &v)
{
    if (v.empty())
        return;

    vector<int> keys;
    keys.reserve(v.size());
    for (const pair<string, string> &p : v)
    {
        keys.push_back(stoi(p.first));
    }

    int min_val = keys[0];
    int max_val = keys[0];
    for (int key : keys)
    {
        if (key < min_val)
            min_val = key;
        if (key > max_val)
            max_val = key;
    }

    vector<int> count(max_val - min_val + 1, 0);

    for (int key : keys)
    {
        count[key - min_val]++;
    }

    for (int i = 1; i < count.size(); ++i)
    {
        count[i] += count[i - 1];
    }

    vector<pair<string, string>> res(v.size());
    for (int i = v.size() - 1; i >= 0; --i)
    {
        int key = keys[i];
        res[--count[key - min_val]] = move(v[i]);
    }

    v = move(res);
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<pair<string, string>> v;
    string line;

    while (getline(cin, line))
    {
        size_t tabPos = line.find('\t');
        if (tabPos == string::npos)
            continue;

        v.emplace_back(
            line.substr(0, tabPos),
            line.substr(tabPos + 1));
    }

    clock_t start = clock();
    countSort(v);
    double timePassed = (double)(clock() - start) / CLOCKS_PER_SEC * 1000.0;

    cerr << "time: " << timePassed << "ms" << endl;
    return 0;
}