#include <iostream>
#include <vector>
#include <random>
#include <set>
#include <map>
#include <chrono>

using namespace std;
using ll = long long;

constexpr ll ITERS = 10'000;
std::mt19937 rnd(chrono::high_resolution_clock::now().time_since_epoch().count());

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
        for (int i = 0; i < n; ++i) {
            ll cnt;
            cin >> cnt;
            stacks[i].resize(cnt);
            for (auto &x : stacks[i]) {
                cin >> x;
            }
        }
        std::vector<std::pair<ll, ll>> moves;

        for (int iter = 0; iter < ITERS; ++iter) {
            ll from = rnd() % stacks.size();
            ll cnt = 0;
            while (!CanBeReturned(from, stacks) && cnt < 100) {
                from = rnd() % stacks.size();
                ++cnt;
            }
            if (cnt == 100) {
                break;
            }
            ll to = rnd() % stacks.size();
            cnt = 0;
            while ((stacks[to].size() >= m || to == from) && cnt < 100) {
                to = rnd() % stacks.size();
                ++cnt;
            }
            if (cnt == 100) {
                break;
            }
            stacks[to].push_back(stacks[from].back());
            stacks[from].pop_back();
        }

        std::cout << moves.size() << std::endl;
        for (auto &x : moves) {
            std::cout << x.first + 1 << ' ' << x.second + 1 << std::endl;
        }
    }
    return 0;
}
