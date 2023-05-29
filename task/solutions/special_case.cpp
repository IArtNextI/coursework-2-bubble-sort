#include <iostream>
#include <vector>
#include <random>
#include <set>
#include <map>

using namespace std;
using ll = long long;

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
        std::set<ll> seen;
        std::vector<std::pair<ll, ll>> moves;
        std::vector<ll> unused(n);
        iota(unused.begin(), unused.end(), 0);
        std::map<ll, ll> target;
        while (true) {
            std::set<ll> empty;
            for (int i = 0; i < n; ++i) {
                if (stacks[i].empty()) {
                    empty.insert(i);
                }
            }
            std::cerr << "Left columns : " << unused.size() << '/' << n << std::endl;
            ll col = -1;
            if (seen.empty()) {
                col = *unused.begin();
                unused.erase(unused.begin());
            }
            else {
                ll maxsc = 0;
                ll ipicked = -1;
                ll picked = -1;
                for (int i = 0; i < unused.size(); ++i) {
                    ll sc = 0;
                    for (auto &y : stacks[unused[i]]) {
                        if (seen.find(y) != seen.end()) {
                            ++sc;
                        }
                    }
                    if (sc > maxsc) {
                        picked = unused[i];
                        maxsc = sc;
                        ipicked = i;
                    }
                }
                if (ipicked == -1) {
                    break;
                }
                unused.erase(unused.begin() + ipicked);
                col = picked;
            }
            if (col == -1) break;
            for (int i = stacks[col].size() - 1; i >= 0; --i) {
                ll x = stacks[col][i];
                if (target.find(x) == target.end() && !empty.empty()) {
                    ll ncol = *empty.begin();
                    empty.erase(empty.begin());
                    seen.insert(x);
                    target[x] = ncol;

                    moves.emplace_back(col, target[x]);
                    stacks[target[x]].push_back(stacks[col].back());
                    stacks[col].pop_back();
                }
                else if (target.find(x) != target.end()) {
                    if (target[x] == col) continue;
                    moves.emplace_back(col, target[x]);
                    stacks[target[x]].push_back(stacks[col].back());
                    stacks[col].pop_back();
                }
                else {
                    break;
                }
            }
        }

        std::cout << moves.size() << std::endl;
        for (auto &x : moves) {
            std::cout << x.first + 1 << ' ' << x.second + 1 << std::endl;
        }
    }
    return 0;
}
