#include <bits/stdc++.h>

#define MINIMP3_IMPLEMENTATION
#include "minimp3.h"
#include "minimp3_ex.h"

using namespace std;
constexpr double PI = 3.14159265358979323846264338327950288;


void fft(vector<complex<double>>& a) {
    int n = a.size();
    static vector<int> rev;
    static vector<complex<double>> roots{ {0,0}, {1,0} };

    if ((int)rev.size() != n) {
        int k = __builtin_ctz(n);
        rev.assign(n, 0);
        for (int i = 0; i < n; i++)
            rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (k - 1));
    }

    if ((int)roots.size() < n) {
        int k = __builtin_ctz(roots.size());
        roots.resize(n);
        while ((1 << k) < n) {
            double angle = 2 * PI / (1 << (k + 1));
            for (int i = 1 << (k - 1); i < (1 << k); i++) {
                roots[2 * i] = roots[i];
                double ang = angle * (2 * i + 1 - (1 << k));
                roots[2 * i + 1] = complex<double>(cos(ang), sin(ang));
            }
            k++;
        }
    }

    for (int i = 0; i < n; i++)
        if (i < rev[i]) swap(a[i], a[rev[i]]);

    for (int k = 1; k < n; k <<= 1) {
        for (int i = 0; i < n; i += 2 * k) {
            for (int j = 0; j < k; j++) {
                complex<double> z = a[i + j + k] * roots[j + k];
                a[i + j + k] = a[i + j] - z;
                a[i + j] += z;
            }
        }
    }
}


vector<short> decode_mp3(const char* filename) {
    mp3dec_t dec;
    mp3dec_file_info_t info;

    if (mp3dec_load(&dec, filename, &info, NULL, NULL)) {
        throw runtime_error("mp3 decode error");
    }

    vector<short> res(info.buffer, info.buffer + info.samples);
    free(info.buffer);
    return res;
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<short> pcm = decode_mp3("input.mp3");

    const int W = 4096;
    const int STEP = 1024;

    static double hann[W];
    for (int i = 0; i < W; i++) {
        hann[i] = 0.5 * (1 - cos(2 * PI * i / (W - 1)));
    }

    int N = pcm.size();
    for (int start = 0; start + W <= N; start += STEP) {
        vector<complex<double>> a(W);

        for (int i = 0; i < W; i++) {
            a[i] = complex<double>(pcm[start + i] * hann[i], 0.0);
        }

        fft(a);

        double mx = 0;
        for (int i = 0; i <= W / 2; i++) {
            mx = max(mx, abs(a[i]));
        }

        cout << fixed << setprecision(20) << mx << "\n";
    }

    return 0;
}
