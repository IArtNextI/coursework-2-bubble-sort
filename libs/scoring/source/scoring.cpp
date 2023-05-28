#include "scoring.hpp"
#include <cmath>

ll calc(std::vector<std::vector <ll>>& a) {
    ll n = a.size();
    std::vector<ll> addr, addc;

    const ll INF = 7e18;

    auto mini = [&] (ll& a, ll b) {
        if (b < a) {
            a = b;
            return true;
        }
        return false;
    };

    auto get = [&](ll i, ll j) {
        return a[i][j] + addr[i] + addc[j];
    };

        ll ttt = INF;
        for (auto &x : a) {
                for (auto &y : x) {
                        mini(ttt, y);
                }
        }
        for (auto &x : a) {
                for (auto &y : x) {
                        y -= ttt;
                }
        }

        addr.resize(n);
        addc.resize(n);

        std::vector<bool> used(n, false);
        std::vector<ll> pair_row(n, -1);
        for (int i = 0; i < n; ++i) {
                std::vector<ll> col_min(n, -1);
                std::vector<ll> p(n, -1);
                ll mine = INF;
                ll col = -1;
                for (int j = 0; j < n; ++j) {
                        if (col_min[j] == -1 || get(col_min[j], j) > get(i, j)) {
                                col_min[j] = i;
                        }
                        if (mini(mine, get(i, j))) {
                                col = j;
                        }
                }

                std::vector<bool> usedr(n), usedc(n);
                usedr[i] = true;
                addr[i] -= mine;
                if (!used[col]) {
                        pair_row[col] = i;
                        used[col] = true;
                }
                else {
                        ll prev = i, prevcol = -1;
                        while (used[col]) {
                                usedc[col] = true;
                                ll t = pair_row[col];
                                p[t] = prev;
                                usedr[t] = true;
                                ll a = INF, b = -1, c = -1;
                                for (int j = 0; j < n; ++j) {
                                        if (!usedc[j]) {
                                                if (col_min[j] == -1 || get(col_min[j], j) > get(t, j)) {
                                                        col_min[j] = t;
                                                }
                                                if (mini(a, get(col_min[j], j))) {
                                                        b = j, c = col_min[j];
                                                }
                                                if (mini(a, get(t, j))) {
                                                        b = j, c = t;
                                                }
                                        }
                                }
                                for (int j = 0; j < n; ++j) {
                                        if (usedr[j]) {
                                                addr[j] -= a;
                                        }
                                        if (usedc[j]) {
                                                addc[j] += a;
                                        }
                                }
                                prevcol = col;
                                col = b;
                                prev = c;
                        }
                        used[col] = true;
                        ll tt = prev;
                        while (tt != -1) {
                                ll flex = 0;
                                for (int i = 0; i < n; ++i) {
                                        if (pair_row[i] == tt) {
                                                flex = i;
                                                break;
                                        }
                                }
                                pair_row[col] = tt;
                                ll pt = p[tt];
                                col = flex;
                                tt = pt;
                        }
                }
        }
        ll res = 0;
        for (int i = 0; i < n; ++i) {
                res += a[pair_row[i]][i] + ttt;
        }
    return res;
}

ll CalculateRawScore(ll n, ll m, ll empty, const std::vector<std::vector<ll>>& state) {
        std::vector<std::vector<ll>> a(n, std::vector<ll>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n - empty; ++j) {
            // count number of elements in stack[i] equal to j;
            a[i][j] = m - std::count(state[i].begin(), state[i].end(), j);
        }
        for (int j = n - empty; j < n; ++j) {
            // count number of elements in stack[i] equal to j;
            a[i][j] = state[i].size();
        }
    }
        return calc(a);
}

ld CalculateScore(ll n, ll m, ll empty, const std::vector<std::vector<ll>>& state) {
    ld score = CalculateRawScore(n, m, empty, state);
    return (n * m - score) / (n * m);
}

ll CalculateSquareScore(ll n, ll m, ll empty, const std::vector<std::vector<ll>>& state) {
        ll res = 0;
        for (auto &x : state) {
                auto cur = x;
                std::sort(cur.begin(), cur.end());
                ll ccur = std::unique(cur.begin(), cur.end()) - cur.begin();
                res += ccur * ccur;
        }
        return res;
}

ld CalculateSquareScoreLd(ll n, ll m, ll empty, ll pscore, ll jscore) {
        ld pres = pscore; // CalculateSquareScore(n, m, empty, pstate);
        ld jres = jscore; // CalculateSquareScore(n, m, empty, jstate);
        // target is n - empty
        // I have a sum of n * m * m
        auto g = [&](ld score) {
                return ((sqrt(n - empty) * m - sqrt(score - n + empty)) / (sqrt(n - empty) * m));
        };
        return std::pow(g(pres) / g(jres), 3);
}

ll CalculateKScore(ll n, ll m, ll empty, const std::vector<std::vector<ll>>& state, ll k) {
        ld res = 0;
        for (auto &x : state) {
                auto cur = x;
                std::sort(cur.begin(), cur.end());
                ll ccur = std::unique(cur.begin(), cur.end()) - cur.begin();
                res += std::pow(1.0l * ccur, 1.0l * k) ;
        }
        return res + 0.5l;
}

ll CalculateMaxScore(ll n, ll m, ll empty, const std::vector<std::vector<ll>>& state) {
        ll res = 0;
        std::vector<ll> cnt;
        for (auto &x : state) {
                cnt.assign(n, 0);
                for (auto &y : x) {
                        ++cnt[y];
                }
                ll cur = m - *std::max_element(cnt.begin(), cnt.end());
                if (cur == m) {
                        continue;
                }
                res += cur * cur;
        }
        return res;
}

ld CalculateMaxScoreD(ll n, ll m, ll empty, const std::vector<std::vector<ll>>& state) {
        ll score = CalculateMaxScore(n, m, empty, state);
        return (1.0l * sqrt(n) * m - sqrt(score)) / (sqrt(n) * m);
}
