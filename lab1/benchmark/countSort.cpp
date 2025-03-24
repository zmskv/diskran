#include <iostream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

struct Pair
{
    string key;
    string value;
};

void countSort(vector<Pair> &v)
{
    if (v.empty())
        return;

    int min = stoi(v[0].key);
    int max = stoi(v[0].key);

    for (auto &p : v)
    {
        int key = stoi(p.key);
        min = std::min(min, key);
        max = std::max(max, key);
    }

    vector<int> count(max - min + 1, 0);

    for (auto &p : v)
    {
        int key = stoi(p.key);
        count[key - min]++;
    }

    for (size_t i = 1; i < count.size(); i++)
    {
        count[i] += count[i - 1];
    }

    vector<Pair> res(v.size());

    for (int i = v.size() - 1; i >= 0; i--)
    {
        int key = stoi(v[i].key);
        res[--count[key - min]] = v[i];
    }

    v = res;
}

int main()
{
    vector<Pair> data;
    string line;

    while (getline(cin, line))
    {
        size_t tabPos = line.find('\t');
        if (tabPos == string::npos)
            continue;

        Pair p;
        p.key = line.substr(0, tabPos);
        p.value = line.substr(tabPos + 1);
        data.push_back(p);
    }

    clock_t start = clock();
    countSort(data);
    double timePassed = (double)(clock() - start) / CLOCKS_PER_SEC * 1000.0;

    cerr << "time: " << timePassed << "ms" << endl;
    return 0;
}