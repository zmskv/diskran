#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

void countSort(vector<pair<string, string>> &v)
{
    if (v.size() == 0)
        return;

    int min = stoi(v[0].first);
    int max = stoi(v[0].first);

    for (pair<string, string> &p : v)
    {
        int key = stoi(p.first);
        if (key < min)
            min = key;
        if (key > max)
            max = key;
    }

    vector<int> count(max - min + 1, 0);

    for (pair<string, string> &p : v)
    {
        int key = stoi(p.first);
        count[key - min]++;
    }

    for (int i = 1; i < count.size(); i++)
    {
        count[i] += count[i - 1];
    }

    vector<pair<string, string>> res(v.size());

    for (int i = v.size() - 1; i >= 0; i--)
    {
        int key = stoi(v[i].first);
        count[key - min]--;
        res[count[key - min]] = v[i];
    }

    v = res;
}

int main()
{
    vector<pair<string, string>> v;
    string line;

    while (getline(cin, line))
    {
        size_t tabPos = line.find('\t');
        if (tabPos == string::npos)
            continue;

        string key = line.substr(0, tabPos);
        string value = line.substr(tabPos + 1);

        v.push_back({key, value});
    }

    countSort(v);

    for (pair<string, string> &p : v)
    {
        cout << p.first << "\t" << p.second << endl;
    }

    return 0;
}