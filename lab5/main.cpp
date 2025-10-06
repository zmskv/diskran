#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

std::vector<int> buildSuffixArray(const std::string &s)
{
    int n = s.length();
    std::vector<int> sa(n), rank(n);
    for (int i = 0; i < n; ++i)
    {
        sa[i] = i;
        rank[i] = static_cast<unsigned char>(s[i]);
    }
    for (int k = 1; k < n; k <<= 1)
    {
        std::stable_sort(sa.begin(), sa.end(), [&](int a, int b)
                         {
            if (rank[a] != rank[b]) return rank[a] < rank[b];
            int ra = (a + k < n) ? rank[a + k] : -1;
            int rb = (b + k < n) ? rank[b + k] : -1;
            return ra < rb; });
        std::vector<int> new_rank(n);
        new_rank[sa[0]] = 0;
        for (int i = 1; i < n; ++i)
        {
            int a = sa[i - 1], b = sa[i];
            int ra0 = rank[a], rb0 = rank[b];
            int ra1 = (a + k < n) ? rank[a + k] : -1;
            int rb1 = (b + k < n) ? rank[b + k] : -1;
            new_rank[b] = new_rank[a] + (ra0 != rb0 || ra1 != rb1);
        }
        rank.swap(new_rank);
        if (rank[sa[n - 1]] == n - 1)
            break;
    }
    return sa;
}

int lowerBound(const std::string &text, const std::string &pattern, const std::vector<int> &sa)
{
    int n = (int)text.size();
    int L = 0, R = n;
    while (L < R)
    {
        int M = (L + R) >> 1;
        int cmp = text.compare(sa[M], pattern.size(), pattern);
        if (cmp < 0)
            L = M + 1;
        else
            R = M;
    }
    return L;
}

int upperBound(const std::string &text, const std::string &pattern, const std::vector<int> &sa)
{
    int n = (int)text.size();
    int L = 0, R = n;
    while (L < R)
    {
        int M = (L + R) >> 1;
        int cmp = text.compare(sa[M], pattern.size(), pattern);
        if (cmp <= 0)
            L = M + 1;
        else
            R = M;
    }
    return L;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string text;
    if (!std::getline(std::cin, text))
        return 0;

    std::vector<int> sa = buildSuffixArray(text);

    std::string pattern;
    int qid = 0;
    while (std::getline(std::cin, pattern))
    {
        ++qid;

        int lb = lowerBound(text, pattern, sa);
        int ub = upperBound(text, pattern, sa);

        if (lb >= ub)
            continue;

        std::vector<int> pos;
        pos.reserve(ub - lb);
        for (int i = lb; i < ub; ++i)
            pos.push_back(sa[i] + 1);
        std::sort(pos.begin(), pos.end());

        std::cout << qid << ": ";
        for (size_t i = 0; i < pos.size(); ++i)
        {
            if (i)
                std::cout << ", ";
            std::cout << pos[i];
        }
        std::cout << "\n";
    }
    return 0;
}
