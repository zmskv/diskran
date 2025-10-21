#include <iostream>
#include <vector>
using namespace std;
using ll = long long;
const ll INF = 1e18;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<ll>> a(n, vector<ll>(m));
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            cin >> a[i][j];
        }
    }

    vector<vector<ll>> dp(n, vector<ll>(m, INF));
    vector<vector<int>> parent(n, vector<int>(m, -1));

    for (int j = 0; j < m; ++j)
        dp[0][j] = a[0][j];

    for (int i = 1; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            for (int delta_j = -1; delta_j <= 1; ++delta_j)
            {
                int prev_j = j + delta_j;
                if (prev_j < 0 || prev_j >= m)
                    continue;
                ll cur_cost = dp[i - 1][prev_j] + a[i][j];
                if (cur_cost < dp[i][j])
                {
                    dp[i][j] = cur_cost;
                    parent[i][j] = prev_j;
                }
            }
        }
    }

    ll best = INF;
    int col = -1;
    for (int j = 0; j < m; ++j)
    {
        if (dp[n - 1][j] < best)
        {
            best = dp[n - 1][j];
            col = j;
        }
    }

    vector<int> path(n);
    for (int i = n - 1; i >= 0; --i)
    {
        path[i] = col;
        col = parent[i][col];
    }

    cout << best << "\n";
    for (int i = 0; i < n; ++i)
    {
        cout << "(" << i + 1 << "," << path[i] + 1 << ")";
        if (i + 1 != n)
            cout << " ";
    }
    cout << "\n";

    return 0;
}
