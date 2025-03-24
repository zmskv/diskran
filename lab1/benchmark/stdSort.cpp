#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>

using namespace std;

struct Pair
{
    string key;
    string value;
};

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
    sort(data.begin(), data.end(), [](const Pair &a, const Pair &b)
         { return stoi(a.key) < stoi(b.key); });
    double timePassed = (double)(clock() - start) / CLOCKS_PER_SEC * 1000.0;

    cerr << "time: " << timePassed << "ms" << endl;
    return 0;
}