#include <cctype>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <unordered_set>
#include <cmath>
#include "scoring.hpp"

using namespace std;
using ll = long long;
using ld = long double;

bool CanBeMoved(size_t index, size_t index2, size_t maxsz, std::vector<std::vector<ll>>& stacks) {
    return !stacks[index].empty() && !stacks[index2].empty() && stacks[index2].size() < maxsz && stacks[index].back() == stacks[index2].back();
}

inline ll EmptyCount(std::vector<std::vector<ll>>& opa) {
    ll res = 0;
    for (auto &x : opa) {
        res += x.empty();
    }
    return res;
}

void GoDeeper(ll n, ll m, ll empty, std::set<std::vector<std::vector<ll>>>& seen, std::vector<std::vector<ll>>& cur, ld& maxe, std::vector<std::vector<ll>>& res, std::vector<std::pair<ll, ll>>& moves, std::vector<std::pair<ll, ll>>& cmoves) {
    if (seen.find(cur) != seen.end()) {
        return;
    }
    seen.insert(cur);
    ld cur_score = (EmptyCount(cur) == empty ? pow(CalculateSquareScoreLd(n, m, empty, CalculateSquareScore(n, m, empty, cur), n - empty), 1.0l / 3) : 1);
    if (cur_score > maxe) {
        maxe = cur_score;
        res = cur;
        moves = cmoves;
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            if (maxe >= 0.8) return;
            if (CanBeMoved(i, j, m, cur) && cur[j].size() < m) {
                cur[j].push_back(cur[i].back());
                cur[i].pop_back();
                cmoves.push_back({i, j});
                GoDeeper(n, m, empty, seen, cur, maxe, res, moves, cmoves);
                cur[i].push_back(cur[j].back());
                cur[j].pop_back();
                cmoves.pop_back();
            }
        }
    }
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

        std::set<std::vector<std::vector<ll>>> seen;
        std::vector<std::vector<ll>> ans;
        std::vector<std::pair<ll, ll>> moves;
        std::vector<std::pair<ll, ll>> cmoves;
        ld maxe = 1;
        GoDeeper(n, m, empty, seen, stacks, maxe, ans, moves, cmoves);

        std::cout << moves.size() << std::endl;
        for (auto &x : moves) {
            std::cout << x.first + 1 << ' ' << x.second + 1 << std::endl;
        }
    }
    return 0;
}

