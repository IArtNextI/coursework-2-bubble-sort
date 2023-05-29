
#include <iostream>
#include <vector>
#include <random>
#include <set>
#include <map>
#include <chrono>

#include "scoring.hpp"

using namespace std;
using ll = long long;
using ld = long double;

constexpr ll ITERS = 10'000;
std::mt19937 rnd(chrono::high_resolution_clock::now().time_since_epoch().count());

ld RnDouble() {
    return rnd() * 1.0l / ((1ll << 32) - 1);
}

bool CanBeReturned(size_t index, std::vector<std::vector<ll>>& stacks) {
    return stacks[index].size() == 1 || stacks[index].size() >= 2 && stacks[index].back() == stacks[index][stacks[index].size() - 2];
}

int main() {
    ll t;
    cin >> t;
    while (t--) {
        ll n, m;
        cin >> n >> m;
        std::vector<std::vector<ll>> stacks(n);
        ll empty = 0;
        for (int i = 0; i < n; ++i) {
            ll cnt;
            cin >> cnt;
            empty += cnt == 0;
            stacks[i].resize(cnt);
            for (auto &x : stacks[i]) {
                cin >> x;
            }
        }
        ld temp = 1;
        static constexpr ld eps = 1e-7;
        ll MAXSCORE = n;
        ll cur = MAXSCORE - CalculateRawScore(n, m, empty, stacks);
        ll steps = 0;
        std::vector<std::pair<ll, ll>> moves;
        while (temp > eps && cur) {
            ++steps;
            ll i = rnd() % n;
            ll cnt = 0;
            while (!CanBeReturned(i, stacks) && cnt < 1000) {
                ++cnt;
                i = rnd() % n;
            }
            if (cnt == 1000) {
                ll i = moves.back().first;
                ll j = moves.back().second;
                stacks[i].push_back(stacks[j].back());
                stacks[j].pop_back();
                continue;
            }
            ll j = rnd() % (n - 1);
            if (j >= i) {
                ++j;
            }
            if (stacks[j].size() == m) {
                continue;
            }
            moves.push_back({ i, j });
            stacks[j].push_back(stacks[i].back());
            stacks[i].pop_back();
            ll ncur = MAXSCORE - CalculateRawScore(n, m, empty, stacks);
            if (ncur < cur || exp((cur - ncur) / temp) >= RnDouble()) {
                cur = ncur;
            }
            else {
                stacks[i].push_back(stacks[j].back());
                stacks[j].pop_back();
                moves.pop_back();
            }
            temp *= 0.9997l;
        }

        std::cout << moves.size() << std::endl;
        for (auto &x : moves) {
            std::cout << x.first + 1 << ' ' << x.second + 1 << std::endl;
        }
    }
    return 0;
}
