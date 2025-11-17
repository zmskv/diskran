#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vector<int> a(N);
    for (int i = 0; i < N; i++) cin >> a[i];

    int cnt[4] = {0};
    for (int x : a) cnt[x]++;

    int zone1_end = cnt[1];
    int zone2_end = cnt[1] + cnt[2];

    int mis[4][4] = {}; 

    for (int i = 0; i < N; i++) {
        int zone;
        if (i < zone1_end) zone = 1;
        else if (i < zone2_end) zone = 2;
        else zone = 3;
        mis[zone][a[i]]++;
    }

    int swaps = 0;

    int direct12 = min(mis[1][2], mis[2][1]);
    swaps += direct12;
    mis[1][2] -= direct12;
    mis[2][1] -= direct12;

    int direct13 = min(mis[1][3], mis[3][1]);
    swaps += direct13;
    mis[1][3] -= direct13;
    mis[3][1] -= direct13;

    int direct23 = min(mis[2][3], mis[3][2]);
    swaps += direct23;
    mis[2][3] -= direct23;
    mis[3][2] -= direct23;

    int remaining = mis[1][2] + mis[1][3] + mis[2][1] + mis[2][3] + mis[3][1] + mis[3][2];
    swaps += (remaining / 3) * 2;

    cout << swaps << "\n";
    return 0;
}
