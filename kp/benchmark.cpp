#include <bits/stdc++.h>
#include <chrono>

#define MINIMP3_IMPLEMENTATION
#include "minimp3.h"
#include "minimp3_ex.h"

using namespace std;
using namespace chrono;
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

double measure_mp3_decode(const char* filename, int iterations = 5) {
    vector<double> times;
    
    for (int iter = 0; iter < iterations; iter++) {
        auto start = high_resolution_clock::now();
        vector<short> pcm = decode_mp3(filename);
        auto end = high_resolution_clock::now();
        
        auto duration = duration_cast<microseconds>(end - start);
        times.push_back(duration.count() / 1000.0);
    }
    
    sort(times.begin(), times.end());
    return times[iterations / 2];
}

double measure_fft(int window_size, int iterations = 100) {
    vector<complex<double>> a(window_size);
    for (int i = 0; i < window_size; i++) {
        a[i] = complex<double>(sin(2 * PI * i / window_size), 0.0);
    }
    
    auto start = high_resolution_clock::now();
    for (int iter = 0; iter < iterations; iter++) {
        vector<complex<double>> b = a;
        fft(b);
    }
    auto end = high_resolution_clock::now();
    
    auto duration = duration_cast<microseconds>(end - start);
    return duration.count() / (1000.0 * iterations);
}

double measure_full_pipeline(const char* filename, int W, int STEP, int iterations = 3) {
    vector<double> times;
    
    for (int iter = 0; iter < iterations; iter++) {
        auto start = high_resolution_clock::now();
        
        vector<short> pcm = decode_mp3(filename);
        
        static double hann[4096];
        if (iter == 0) {
            for (int i = 0; i < W; i++) {
                hann[i] = 0.5 * (1 - cos(2 * PI * i / (W - 1)));
            }
        }
        
        int N = pcm.size();
        for (int start_pos = 0; start_pos + W <= N; start_pos += STEP) {
            vector<complex<double>> a(W);
            
            for (int i = 0; i < W; i++) {
                a[i] = complex<double>(pcm[start_pos + i] * hann[i], 0.0);
            }
            
            fft(a);
            
            double mx = 0;
            for (int i = 0; i <= W / 2; i++) {
                mx = max(mx, abs(a[i]));
            }
        }
        
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        times.push_back(duration.count() / 1000.0);
    }
    
    sort(times.begin(), times.end());
    return times[iterations / 2];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cout << "=== Performance Benchmarks ===\n\n";
    
    cout << "1. MP3 Decoding:\n";
    try {
        double time = measure_mp3_decode("input.mp3");
        cout << "   Average time: " << fixed << setprecision(2) << time << " ms\n";
    } catch (const exception& e) {
        cout << "   Error: " << e.what() << "\n";
    }
    
    cout << "\n2. FFT Performance:\n";
    vector<int> window_sizes = {512, 1024, 2048, 4096, 8192};
    for (int W : window_sizes) {
        double time = measure_fft(W);
        cout << "   Window size " << W << ": " << fixed << setprecision(3) << time << " ms\n";
    }
    
    cout << "\n3. Full Pipeline (decoding + processing):\n";
    try {
        const int W = 4096;
        const int STEP = 1024;
        double time = measure_full_pipeline("input.mp3", W, STEP);
        cout << "   Processing time: " << fixed << setprecision(2) << time << " ms\n";
        
        vector<short> pcm = decode_mp3("input.mp3");
        int N = pcm.size();
        int windows = (N - W) / STEP + 1;
        cout << "   Number of windows: " << windows << "\n";
        cout << "   Time per window: " << fixed << setprecision(3) << time / windows << " ms\n";
    } catch (const exception& e) {
        cout << "   Error: " << e.what() << "\n";
    }
    
    cout << "\n4. Comparison of different window sizes (full pipeline):\n";
    try {
        vector<short> pcm = decode_mp3("input.mp3");
        vector<pair<int, int>> configs = {{2048, 512}, {4096, 1024}, {8192, 2048}};
        
        for (auto [W, STEP] : configs) {
            double hann[8192];
            for (int i = 0; i < W; i++) {
                hann[i] = 0.5 * (1 - cos(2 * PI * i / (W - 1)));
            }
            
            int N = pcm.size();
            int windows = 0;
            
            auto start = high_resolution_clock::now();
            for (int start_pos = 0; start_pos + W <= N; start_pos += STEP) {
                vector<complex<double>> a(W);
                
                for (int i = 0; i < W; i++) {
                    a[i] = complex<double>(pcm[start_pos + i] * hann[i], 0.0);
                }
                
                fft(a);
                
                double mx = 0;
                for (int i = 0; i <= W / 2; i++) {
                    mx = max(mx, abs(a[i]));
                }
                windows++;
            }
            auto end = high_resolution_clock::now();
            
            auto duration = duration_cast<microseconds>(end - start);
            double time_ms = duration.count() / 1000.0;
            
            cout << "   Window " << W << " / step " << STEP << ": " 
                 << fixed << setprecision(2) << time_ms << " ms (" 
                 << windows << " windows, " << fixed << setprecision(3) 
                 << time_ms / windows << " ms/window)\n";
        }
    } catch (const exception& e) {
        cout << "   Error: " << e.what() << "\n";
    }
    
    return 0;
}

