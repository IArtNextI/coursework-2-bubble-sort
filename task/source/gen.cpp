#include <cctype>
#include <cstdio>
#include <fstream>
#include <string>
#define _CRT_SECURE_NO_WARNINGS

#include <algorithm>
#include <iostream>
#include <numeric>
#include <unordered_set>

#include "testlib.h"

using ll = long long;
using ld = long double;

ld RnDouble() {
    return rnd.next();
};

bool CanBeReturned(size_t index, std::vector<std::vector<ll>>& stacks) {
    return stacks[index].size() == 1 || stacks[index].size() >= 2 && stacks[index].back() == stacks[index][stacks[index].size() - 2];
}

bool Finalized(std::vector<std::vector<ll>>& stacks, ll empty) {
    ll cnt = 0;
    for (auto &x : stacks) {
        cnt += x.empty();
    }
    return cnt == empty;
}

void TryToFinalize(std::vector<std::vector<ll>>& stacks, ll empty, ll bubbles) {
    while (!Finalized(stacks, empty)) {
        ll from = rnd.next(0ul, stacks.size() - 1);
        ll cnt = 0;
        while (!CanBeReturned(from, stacks) && cnt < 100) {
            from = rnd.next(0ul, stacks.size() - 1);
            ++cnt;
        }
        if (cnt == 100) {
            std::cerr << "Having a trouble generating a move, aboriting..." << std::endl;
            exit(1);
        }
        ll to = rnd.next(0ul, stacks.size() - 1);
        cnt = 0;
        while ((stacks[to].size() >= bubbles || to == from) && cnt < 100) {
            to = rnd.next(0ul, stacks.size() - 1);
            ++cnt;
        }
        if (cnt == 100) {
            std::cerr << "Having a trouble generating a move, aboriting..." << std::endl;
            exit(1);
        }
        stacks[to].push_back(stacks[from].back());
        stacks[from].pop_back();
    }
}

#define WriteAnswer()                                                                           \
    std::cout << full + empty << ' ' << bubbles << std::endl;                                   \
    std::vector<ll> shuffler(full);                                                             \
    std::iota(shuffler.begin(), shuffler.end(), 0);                                             \
    shuffle(shuffler.begin(), shuffler.end());                                                  \
    for (auto &x : stacks) {                                                                    \
        if (x.empty()) {                                                                        \
            std::cout << 0 << '\n';                                                             \
            continue;                                                                           \
        }                                                                                       \
        std::cout << x.size() << ' ';                                                           \
        for (int i = 0; i < x.size(); ++i) { /* TODO : final position must be fully separated */\
            std::cout << shuffler[x[i]] << " \n"[i == x.size() - 1];                            \
        }                                                                                       \
    }

int Gen0(ll full, ll empty, ll bubbles, ll iterations) {
    std::vector<std::vector<ll>> stacks(full + empty);
    for (int i = 0; i < full; ++i) {
        stacks[i].resize(bubbles, i);
    }
    bool abort = false;
    while (iterations--) {
        ll from = rnd.next(0ul, stacks.size() - 1);
        ll cnt = 0;
        while (!CanBeReturned(from, stacks) && cnt < 100) {
            from = rnd.next(0ul, stacks.size() - 1);
            ++cnt;
        }
        if (cnt == 100) {
            std::cerr << "Having a trouble generating a move, aboriting..." << std::endl;
            abort = true;
            break;
        }
        ll to = rnd.next(0ul, stacks.size() - 1);
        cnt = 0;
        while ((stacks[to].size() >= bubbles || to == from) && cnt < 100) {
            to = rnd.next(0ul, stacks.size() - 1);
            ++cnt;
        }
        if (cnt == 100) {
            std::cerr << "Having a trouble generating a move, aboriting..." << std::endl;
            abort = true;
            break;
        }
        stacks[to].push_back(stacks[from].back());
        stacks[from].pop_back();
    }
    TryToFinalize(stacks, empty, bubbles);
    if (abort) {
        return 1;
    }
    WriteAnswer();
    return 0;
}

#include "scoring.hpp"

int Gen1(ll full, ll empty, ll bubbles) {
    std::vector<std::vector<ll>> stacks(full + empty);
    for (int i = 0; i < full; ++i) {
        stacks[i].resize(bubbles, i);
    }
    std::string cur;
    auto print_state = [&]() {
        std::cerr << "Current state : " << std::endl;
        ll bucket_num = 1;
        for (auto &x : stacks) {
            std::cerr << bucket_num++ << " : ";
            for (auto &y : x) {
                std::cerr << y << ' ';
            }
            std::cerr << std::endl;
        }
        ll score = CalculateRawScore(full + empty, bubbles, empty, stacks);
        std::cerr << score << std::endl;
        std::cerr << "Current score : " << (score == 0 ?  10 : 10 * CalculateScore(full + empty, bubbles, empty, stacks)) << std::endl;
    };
    print_state();
    std::cerr << "Enter a move:" << std::endl;
    std::cin >> cur;
    while (cur != "q") {
        ll f = std::stoll(cur);
        std::cin >> cur;
        ll s = std::stoll(cur);
        --f; --s;
        if (!CanBeReturned(f, stacks) || stacks[s].size() == bubbles) {
            std::cerr << "Can't perform" << std::endl;
        }
        else {
            stacks[s].push_back(stacks[f].back());
            stacks[f].pop_back();
        }
        print_state();
        std::cerr << "Enter a move:" << std::endl;
        std::cin >> cur;
    }
    WriteAnswer();
    return 0;
}

int Gen2(ll full, ll empty, ll bubbles) {
    std::vector<std::vector<ll>> stacks(full + empty);
    std::vector<ll> cnt(full * bubbles);
    for (int i = 0; i < bubbles; ++i) {
        std::iota(cnt.begin() + i * full, cnt.begin() + (i + 1) * full, 0);
    }
    for (int i = 1; i < cnt.size(); ++i) {
        std::swap(cnt[i], cnt[rnd.next(i)]);
    }
    for (int i = 0; i < full; ++i) {
        stacks[i].resize(bubbles);
        for (int j = 0; j < bubbles; ++j) {
            stacks[i][j] = cnt[i * bubbles + j];
        }
    }
    for (int i = 1; i < full + empty; ++i) {
        stacks[i].swap(stacks[rnd.next(i)]);
    }
    WriteAnswer();
    return 0;
}

int Gen3(ll full, ll empty, ll bubbles) {
    std::vector<std::vector<ll>> stacks(full + empty);
    for (int i = 0; i < full; ++i) {
        stacks[i].resize(bubbles, i);
    }
    ld temp = 1;
    static constexpr ld eps = 1e-7;
    ll MAXSCORE = (full + empty) * bubbles;
    ll cur = MAXSCORE - CalculateRawScore(empty + full, bubbles, empty, stacks);
    ll steps = 0;
    ll n = full + empty;
    std::vector<std::pair<ll, ll>> moves;
    while (temp > eps && cur) {
        ++steps;
        ll i = rnd.next(n);
        ll cnt = 0;
        while (!CanBeReturned(i, stacks) && cnt < 1000) {
            ++cnt;
            i = rnd.next(n);
        }
        if (cnt == 1000) {
            ll i = moves.back().first;
            ll j = moves.back().second;
            stacks[i].push_back(stacks[j].back());
            stacks[j].pop_back();
            continue;
        }
        ll j = rnd.next(n - 1);
        if (j >= i) {
            ++j;
        }
        if (stacks[j].size() == bubbles) {
            continue;
        }
        moves.push_back({ i, j });
        stacks[j].push_back(stacks[i].back());
        stacks[i].pop_back();
        ll ncur = MAXSCORE - CalculateRawScore(full + empty, bubbles, empty, stacks);
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
    std::cerr << "Generated in " << steps << " steps" << std::endl;
    WriteAnswer();
    return 0;
}

inline ll EmptyCount(std::vector<std::vector<ll>>& opa) {
    ll res = 0;
    for (auto &x : opa) {
        res += x.empty();
    }
    return res;
}

void GoDeeper(ll n, ll m, ll empty, std::set<std::vector<std::vector<ll>>>& seen, std::vector<std::vector<ll>>& cur, ld& maxe, std::vector<std::vector<ll>>& res, bool& satisfied, std::vector<std::pair<ll, ll>>& moves, std::vector<std::pair<ll, ll>>& cmoves) {
    if (seen.find(cur) != seen.end()) {
        return;
    }
//    std::cerr << "--------------------------" << std::endl;
//    for (auto &x : cur) {
//        for (auto &y : x) {
//            std::cerr << y << ' ';
//        }
//        std::cerr << std::endl;
//    }
//    std::cerr << "--------------------------" << std::endl;
    seen.insert(cur);
    ld cur_score = (EmptyCount(cur) == empty ? pow(CalculateSquareScoreLd(n, m, empty, CalculateSquareScore(n, m, empty, cur), n - empty), 1.0l / 3) : 1);
    if (cur_score < maxe) {
        // std::cerr << maxe << std::endl;
        // for (auto &x : cur) {
        //     std::cerr << x.size() << ' ';
        //     for (auto &y : x) {
        //         std::cerr << y << ' ';
        //     }
        //     std::cerr << std::endl;
        // }
        maxe = cur_score;
        res = cur;
        moves = cmoves;
        ll resp = 0;
//        std::cerr << "Ok?" << std::endl;
//        std::cin >> resp;
        if (resp == 1) {
            satisfied = true;
            return;
        }
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            if (maxe <= 0.33 || satisfied) return;
            if (CanBeReturned(i, cur) && cur[j].size() < m) {
                cur[j].push_back(cur[i].back());
                cur[i].pop_back();
                cmoves.push_back({i, j});
                GoDeeper(n, m, empty, seen, cur, maxe, res, satisfied, moves, cmoves);
                cur[i].push_back(cur[j].back());
                cur[j].pop_back();
                cmoves.pop_back();
            }
        }
    }
}

int Gen4(ll full, ll empty, ll bubbles) {
    std::vector<std::vector<ll>> stacks(full + empty);
    for (int i = 0; i < full; ++i) {
        stacks[i].resize(bubbles, i);
    }
    std::set<std::vector<std::vector<ll>>> seen;
    std::vector<std::vector<ll>> ans;
    std::vector<std::pair<ll, ll>> moves;
    std::vector<std::pair<ll, ll>> cmoves;
    ld maxe = 1;
    bool ok = false;
    GoDeeper(full + empty, bubbles, empty, seen, stacks, maxe, ans, ok, moves, cmoves);
    stacks = ans;
    WriteAnswer();
    std::string filename = std::to_string(full) + "_" + std::to_string(empty) + "_" + std::to_string(bubbles) + ".a";
    std::ofstream out(filename);
    out << moves.size() << std::endl;
    reverse(moves.begin(), moves.end());
    for (auto &x : moves) {
        out << x.second + 1 << ' ' << x.first + 1 << std::endl;
    }
    return 0;
}

std::vector<ll> LoadEmpty(const std::vector<std::vector<ll>>& stacks) {
    std::vector<ll> empty_stacks;
    for (int i = 0; i < stacks.size(); ++i) {
        if (stacks[i].empty()) {
            empty_stacks.push_back(i);
        }
    }
    return empty_stacks;
}

int Gen5(ll full, ll empty, ll bubbles) {
    std::vector<std::vector<ll>> stacks(full + empty);
    for (int i = 0; i < full; ++i) {
        stacks[i].resize(bubbles, i);
    }
    std::vector<std::pair<ll, ll>> made_moves;
    std::vector<ll> unused(full);
    iota(unused.begin(), unused.end(), 0);
    auto gen_subset = [&](std::vector<ll>& available) {
        ll size = rnd.next(std::min<ll>(empty - 1, available.size() - 1)) + 2;
        while (available.size() - size == 1) {
            size = rnd.next(std::min<ll>(empty - 1, available.size() - 1)) + 2;
        }
        std::vector<ll> res;
        for (int i = 0; i < size; ++i) {
            ll ind = rnd.next(available.size());
            res.push_back(available[ind]);
            available.erase(available.begin() + ind);
        }
        return res;
    };
    while (unused.size() > 0) {
        auto empty_stacks = LoadEmpty(stacks);
        auto cur = gen_subset(unused);
        std::vector<ll> all_bubbles;

        for (auto &x : cur) {
            for (auto &y : stacks[x]) {
                all_bubbles.push_back(y);
            }
            stacks[x].clear();
        }
        shuffle(all_bubbles.begin(), all_bubbles.end());
        shuffle(empty_stacks.begin(), empty_stacks.end());
        for (int i = 0; i < cur.size(); ++i) {
            for (int j = 0; j < bubbles; ++j) {
                made_moves.emplace_back(all_bubbles.back(), empty_stacks.back());
                stacks[empty_stacks.back()].push_back(all_bubbles.back());
                all_bubbles.pop_back();
            }
            empty_stacks.pop_back();
        }
    }
    WriteAnswer();
    std::ofstream out(std::to_string(full) + "_" + std::to_string(empty) + "_" + std::to_string(bubbles) + ".a");
    out << made_moves.size() << std::endl;
    reverse(made_moves.begin(), made_moves.end());
    for (auto &x : made_moves) {
        out << x.second + 1 << ' ' << x.first + 1 << std::endl;
    }
    out.close();

    return 0;
}

int Gen6(ll full, ll empty, ll bubbles) {
    //--------------------------------------------------------------------------
    // Assume you have 2k empty stacks
    // Then shuffle can look like :
    //   Take <= k different stacks = V
    //   Take <= k different stacks = U
    // Then we can shuffle like this :
    //    interplace all of V into 2k stacks
    //    Next, layer half of U into k stacks and preserve half. Should look like
    //    this :
    //    |   |   |
    //    |   | V |
    //    | V | U |
    //    +---+---+
    // After that repeat iterations until you get something like
    // this:
    //    |   |   |   |   |
    //    | S | V | U | W |
    //    | V | U | W | S |
    //    +---+---+---+---+
    //
    // This means that 4k needs to divide n to be perfectly shuflled
    // However, I will probably just take random number between 1 and empty / 4
    // This also gives and idea for a better generator where constant 4 is not fixed but is also random
    //--------------------------------------------------------------------------
    if (empty % 2) {
        std::cerr << "Expected to receive empty congruent to 0 modulo 2" << std::endl;
        return 1;
    }
    ll n = full + empty;
    ll k = empty / 2;
    if (n % k) {
        std::cerr << "Expected to receive full + empty congruent to 0 modulo k" << std::endl;
        return 1;
    }
    ll all_bubbles_count = k * bubbles;
    if (all_bubbles_count % 2) {
        std::cerr << "Expected to have k * bubbles to be even" << std::endl;
        return 1;
    }
    std::vector<std::vector<ll>> stacks(full + empty);
    for (int i = 0; i < full; ++i) {
        stacks[i].resize(bubbles, i);
    }

    std::vector<ll> unused(full);
    iota(unused.begin(), unused.end(), 0);
    auto gen_subset = [](std::vector<ll>& available, ll size) {
        if (size > available.size()) {
            std::cerr << "Request to generate set of size = " << size << " while " << available.size() << " is available";
            exit(1);
        }
        std::vector<ll> res;
        for (int i = 0; i < size; ++i) {
            ll ind = rnd.next(available.size());
            res.push_back(available[ind]);
            available.erase(available.begin() + ind);
        }
        return res;
    };

    std::vector<std::vector<ll>> sets((n - empty) / k);
    for (int i = 0; i < sets.size(); ++i) {
        sets[i] = gen_subset(unused, k);
    }
    std::vector<std::vector<ll>> sets_bubbles(n - empty);
    for (int i = 0; i < sets.size(); ++i) {
        sets_bubbles[i].reserve(all_bubbles_count);
        for (auto &x : sets[i]) {
            for (auto &y : stacks[x]) {
                sets_bubbles[i].push_back(y);
            }
        }
    }
/*  std::cerr << "Split went like this : " << std::endl;
    for (auto &x : sets) {
        for (auto &y : x) {
            std::cerr << y << ' ';
        }
        std::cerr << std::endl;
    }
    std::cerr << "Split print end" << std::endl;*/

    std::vector<std::vector<ll>> used_sets(sets.size());
    std::vector<std::pair<ll, ll>> made_moves;
    for (int i = 0; i < sets.size(); ++i) {
//        std::cerr << "CURRENT STACK STATE" << std::endl;
/*        for (auto &x : stacks) {
            std::cerr << x.size() << ' ';
            for (auto &y : x) {
                std::cerr << y << ' ';
            }
            std::cerr << std::endl;
        }*/
//        std::cerr << "STACK STATE END" << std::endl;
        shuffle(sets_bubbles[i].begin(), sets_bubbles[i].end());
        auto empty_stacks = LoadEmpty(stacks);
        shuffle(empty_stacks.begin(), empty_stacks.end());
        while (empty_stacks.size() > k) {
            empty_stacks.pop_back();
        }
        std::vector<ll> used;
//        auto printt = [&]() {
//            std::cerr << "MOVE : " << made_moves.back().first << " " << made_moves.back().second << std::endl;
//        };
        if (i == 0) {
            for (int j = 0; j < all_bubbles_count / 2; ++j) {
                ll taken_bubble = sets_bubbles[0][j];
                stacks[taken_bubble].pop_back();
                stacks[empty_stacks.back()].push_back(taken_bubble);
                made_moves.push_back({ taken_bubble, empty_stacks.back() });
                if (stacks[empty_stacks.back()].size() == bubbles / 2) {
                    used.push_back(empty_stacks.back());
                    empty_stacks.pop_back();
                }
            }
            used_sets[0] = used;
        }
        else {
            for (int j = 0; j < all_bubbles_count / 2; ++j) {
                ll taken_bubble = sets_bubbles[i][j];
                stacks[taken_bubble].pop_back();
                stacks[empty_stacks.back()].push_back(taken_bubble);
                made_moves.push_back({ taken_bubble, empty_stacks.back() });
                // printt();
                if (stacks[empty_stacks.back()].size() == bubbles / 2) {
                    used.push_back(empty_stacks.back());
                    empty_stacks.pop_back();
                }
            }
            used_sets[i] = used;
            for (int j = all_bubbles_count / 2; j < all_bubbles_count; ++j) {
                ll taken_bubble = sets_bubbles[i - 1][j];
                stacks[taken_bubble].pop_back();
                stacks[used_sets[i].back()].push_back(taken_bubble);
                made_moves.push_back({ taken_bubble, used_sets[i].back() });
                // printt();
                if (stacks[used_sets[i].back()].size() == bubbles) {
                    used_sets[i].pop_back();
                }
            }
        }
        if (i == sets.size() - 1) {
            for (int j = all_bubbles_count / 2; j < all_bubbles_count; ++j) {
                ll taken_bubble = sets_bubbles[i][j];
                stacks[taken_bubble].pop_back();
                stacks[used_sets[0].back()].push_back(taken_bubble);
                made_moves.push_back({ taken_bubble, used_sets[0].back() });
                if (stacks[used_sets[0].back()].size() == bubbles) {
                    used.push_back(used_sets[0].back());
                    used_sets[0].pop_back();
                }
            }
        }
    }
    WriteAnswer();
    std::ofstream out(std::to_string(full) + "_" + std::to_string(empty) + "_" + std::to_string(bubbles) + ".a");
    out << made_moves.size() << std::endl;
    reverse(made_moves.begin(), made_moves.end());
    for (auto &x : made_moves) {
        out << x.second + 1 << ' ' << x.first + 1 << std::endl;
    }
    out.close();
    return 0;
}

int Gen7(ll full, ll empty, ll bubbles, ll crop) {
    //--------------------------------------------------------------------------
    // Gen6 + randomly crop empty
    //--------------------------------------------------------------------------
/*    if (empty % 2) {
        std::cerr << "Expected to receive empty congruent to 0 modulo 2" << std::endl;
        return 1;
    }
    ll n = full + empty;
    ll k = empty / 2;
    if (n % k) {
        std::cerr << "Expected to receive full + empty congruent to 0 modulo k" << std::endl;
        return 1;
    }
    ll all_bubbles_count = k * bubbles;
    if (all_bubbles_count % 2) {
        std::cerr << "Expected to have k * bubbles to be even" << std::endl;
        return 1;
    }
    std::vector<std::vector<ll>> stacks(full + empty);
    for (int i = 0; i < full; ++i) {
        stacks[i].resize(bubbles, i);
    }

    std::vector<ll> unused(full);
    iota(unused.begin(), unused.end(), 0);
    auto gen_subset = [](std::vector<ll>& available, ll size) {
        if (size > available.size()) {
            std::cerr << "Request to generate set of size = " << size << " while " << available.size() << " is available";
            exit(1);
        }
        std::vector<ll> res;
        for (int i = 0; i < size; ++i) {
            ll ind = rnd.next(available.size());
            res.push_back(available[ind]);
            available.erase(available.begin() + ind);
        }
        return res;
    };

    std::vector<std::vector<ll>> sets((n - empty) / k);
    for (int i = 0; i < sets.size(); ++i) {
        sets[i] = gen_subset(unused, k);
    }
    std::vector<std::vector<ll>> sets_bubbles((n - empty) / k);
    for (int i = 0; i < sets.size(); ++i) {
        sets_bubbles[i].reserve(all_bubbles_count);
        for (auto &x : sets[i]) {
            for (auto &y : stacks[x]) {
                sets_bubbles[i].push_back(y);
            }
        }
    }
    std::vector<std::vector<ll>> used_sets(sets.size());
    std::vector<std::pair<ll, ll>> made_moves;
    ll border = rnd.next(all_bubbles_count / 3, all_bubbles_count / 2);
    for (int i = 0; i < sets.size(); ++i) {
        shuffle(sets_bubbles[i].begin(), sets_bubbles[i].end());
        auto empty_stacks = LoadEmpty(stacks);
        shuffle(empty_stacks.begin(), empty_stacks.end());
        while (empty_stacks.size() > k) {
            empty_stacks.pop_back();
        }
        std::vector<ll> used;
        if (i == 0) {
            for (int j = 0; j < border; ++j) {
                ll taken_bubble = sets_bubbles[0][j];
                stacks[taken_bubble].pop_back();
                stacks[empty_stacks.back()].push_back(taken_bubble);
                made_moves.push_back({ taken_bubble, empty_stacks.back() });
                if (stacks[empty_stacks.back()].size() == bubbles / 2) {
                    used.push_back(empty_stacks.back());
                    empty_stacks.pop_back();
                }
            }
            used_sets[0] = used;
        }
        else {
            std::vector<bool> flags(all_bubbles_count, false);
            for (int i = 0; i < border; ++i) {
                flags[i] = true;
            }
            shuffle(flags.begin(), flags.end());
            for (int x = 0; x < all_bubbles_count; ++x) {
                for (int j = 0; j < border; ++j) {
                    ll taken_bubble = sets_bubbles[i][j];
                    stacks[taken_bubble].pop_back();
                    stacks[empty_stacks.back()].push_back(taken_bubble);
                    made_moves.push_back({ taken_bubble, empty_stacks.back() });
                    if (stacks[empty_stacks.back()].size() == bubbles / 2) {
                        used.push_back(empty_stacks.back());
                        empty_stacks.pop_back();
                    }
                }
                used_sets[i] = used;
                for (int j = border; j < all_bubbles_count; ++j) {
                    ll taken_bubble = sets_bubbles[i - 1][j];
                    stacks[taken_bubble].pop_back();
                    stacks[used_sets[i].back()].push_back(taken_bubble);
                    made_moves.push_back({ taken_bubble, used_sets[i].back() });
                    if (stacks[used_sets[i].back()].size() == bubbles) {
                        used_sets[i].pop_back();
                    }
                }
            }
        }
        if (i == sets.size() - 1) {
            for (int j = border; j < all_bubbles_count; ++j) {
                ll taken_bubble = sets_bubbles[i][j];
                stacks[taken_bubble].pop_back();
                stacks[used_sets[0].back()].push_back(taken_bubble);
                made_moves.push_back({ taken_bubble, used_sets[0].back() });
                if (stacks[used_sets[0].back()].size() == bubbles) {
                    used.push_back(used_sets[0].back());
                    used_sets[0].pop_back();
                }
            }
        }
    }
    empty -= crop;
    std::vector<ll> empty_v;
    for (int i = 0; i < stacks.size(); ++i) {
        if (stacks[i].empty()) {
            empty_v.push_back(i);
        }
    }
    auto to_crop = gen_subset(empty_v, crop);
    sort(to_crop.begin(), to_crop.end());
    for (int i = to_crop.size() - 1; i >= 0; --i) {
        stacks.erase(stacks.begin() + to_crop[i]);
    }*/
        if (empty % 2) {
        std::cerr << "Expected to receive empty congruent to 0 modulo 2" << std::endl;
        return 1;
    }
    ll n = full + empty;
    ll k = empty / 2;
    if (n % k) {
        std::cerr << "Expected to receive full + empty congruent to 0 modulo k" << std::endl;
        return 1;
    }
    ll all_bubbles_count = k * bubbles;
    if (all_bubbles_count % 2) {
        std::cerr << "Expected to have k * bubbles to be even" << std::endl;
        return 1;
    }
    std::vector<std::vector<ll>> stacks(full + empty);
    for (int i = 0; i < full; ++i) {
        stacks[i].resize(bubbles, i);
    }

    std::vector<ll> unused(full);
    iota(unused.begin(), unused.end(), 0);
    auto gen_subset = [](std::vector<ll>& available, ll size) {
        if (size > available.size()) {
            std::cerr << "Request to generate set of size = " << size << " while " << available.size() << " is available";
            exit(1);
        }
        std::vector<ll> res;
        for (int i = 0; i < size; ++i) {
            ll ind = rnd.next(available.size());
            res.push_back(available[ind]);
            available.erase(available.begin() + ind);
        }
        return res;
    };

    std::vector<std::vector<ll>> sets((n - empty) / k);
    for (int i = 0; i < sets.size(); ++i) {
        sets[i] = gen_subset(unused, k);
    }
    std::vector<std::vector<ll>> sets_bubbles(n - empty);
    for (int i = 0; i < sets.size(); ++i) {
        sets_bubbles[i].reserve(all_bubbles_count);
        for (auto &x : sets[i]) {
            for (auto &y : stacks[x]) {
                sets_bubbles[i].push_back(y);
            }
        }
    }
/*  std::cerr << "Split went like this : " << std::endl;
    for (auto &x : sets) {
        for (auto &y : x) {
            std::cerr << y << ' ';
        }
        std::cerr << std::endl;
    }
    std::cerr << "Split print end" << std::endl;*/

    std::vector<std::vector<ll>> used_sets(sets.size());
    std::vector<std::pair<ll, ll>> made_moves;
    for (int i = 0; i < sets.size(); ++i) {
//        std::cerr << "CURRENT STACK STATE" << std::endl;
/*        for (auto &x : stacks) {
            std::cerr << x.size() << ' ';
            for (auto &y : x) {
                std::cerr << y << ' ';
            }
            std::cerr << std::endl;
        }*/
//        std::cerr << "STACK STATE END" << std::endl;
        shuffle(sets_bubbles[i].begin(), sets_bubbles[i].end());
        auto empty_stacks = LoadEmpty(stacks);
        shuffle(empty_stacks.begin(), empty_stacks.end());
        while (empty_stacks.size() > k) {
            empty_stacks.pop_back();
        }
        std::vector<ll> used;
//        auto printt = [&]() {
//            std::cerr << "MOVE : " << made_moves.back().first << " " << made_moves.back().second << std::endl;
//        };
        if (i == 0) {
            for (int j = 0; j < all_bubbles_count / 2; ++j) {
                ll taken_bubble = sets_bubbles[0][j];
                stacks[taken_bubble].pop_back();
                stacks[empty_stacks.back()].push_back(taken_bubble);
                made_moves.push_back({ taken_bubble, empty_stacks.back() });
                if (stacks[empty_stacks.back()].size() == bubbles / 2) {
                    used.push_back(empty_stacks.back());
                    empty_stacks.pop_back();
                }
            }
            used_sets[0] = used;
        }
        else {
            for (int j = 0; j < all_bubbles_count / 2; ++j) {
                ll taken_bubble = sets_bubbles[i][j];
                stacks[taken_bubble].pop_back();
                stacks[empty_stacks.back()].push_back(taken_bubble);
                made_moves.push_back({ taken_bubble, empty_stacks.back() });
                // printt();
                if (stacks[empty_stacks.back()].size() == bubbles / 2) {
                    used.push_back(empty_stacks.back());
                    empty_stacks.pop_back();
                }
            }
            used_sets[i] = used;
            for (int j = all_bubbles_count / 2; j < all_bubbles_count; ++j) {
                ll taken_bubble = sets_bubbles[i - 1][j];
                stacks[taken_bubble].pop_back();
                stacks[used_sets[i].back()].push_back(taken_bubble);
                made_moves.push_back({ taken_bubble, used_sets[i].back() });
                // printt();
                if (stacks[used_sets[i].back()].size() == bubbles) {
                    used_sets[i].pop_back();
                }
            }
        }
        if (i == sets.size() - 1) {
            for (int j = all_bubbles_count / 2; j < all_bubbles_count; ++j) {
                ll taken_bubble = sets_bubbles[i][j];
                stacks[taken_bubble].pop_back();
                stacks[used_sets[0].back()].push_back(taken_bubble);
                made_moves.push_back({ taken_bubble, used_sets[0].back() });
                if (stacks[used_sets[0].back()].size() == bubbles) {
                    used.push_back(used_sets[0].back());
                    used_sets[0].pop_back();
                }
            }
        }
    }
    empty -= crop;
    std::vector<ll> empty_v;
    for (int i = 0; i < stacks.size(); ++i) {
        if (stacks[i].empty()) {
            empty_v.push_back(i);
        }
    }
    auto to_crop = gen_subset(empty_v, crop);
    sort(to_crop.begin(), to_crop.end());
    for (int i = to_crop.size() - 1; i >= 0; --i) {
        stacks.erase(stacks.begin() + to_crop[i]);
    }
    WriteAnswer();
    return 0;
}

int main(int argc, char** argv) {
    registerGen(argc, argv, 1);
    ll intensity = opt<ll>("intensity");
    //------------------------------------------
    // 0 = randomly move for some time, then randomly move to finalize
    // 1 = interactive
    // 2 = fully random
    // 3 = annealing
    // 4 = full analisys for small tests
    // 5 = split into groups of size = k <= empty, then fill k empty columns randomizing what we put
    // 6 = read descruption of Gen6
    //------------------------------------------
    if (intensity == 0) {
        ll full = opt<ll>("full");
        ll empty = opt<ll>("empty");
        ll bubbles = opt<ll>("bubbles");
        ll iterations = opt<ll>("iterations");
        return Gen0(full, empty, bubbles, iterations);
    }
    else if (intensity == 1) {
        // -------------------------
        // Idea
        // Select packs of columns of different sizes <= empty, interplace randomly
        // -------------------------
        ll full = opt<ll>("full");
        ll empty = opt<ll>("empty");
        ll bubbles = opt<ll>("bubbles");
        return Gen1(full, empty, bubbles);
    }
    else if (intensity == 2) {
        // -------------------------
        // Idea
        // Just generate random junk with correct amounts of balls of each color
        // -------------------------
        ll full = opt<ll>("full");
        ll empty = opt<ll>("empty");
        ll bubbles = opt<ll>("bubbles");
        return Gen2(full, empty, bubbles);
    }
    else if (intensity == 3) {
        ll full = opt<ll>("full");
        ll empty = opt<ll>("empty");
        ll bubbles = opt<ll>("bubbles");
        return Gen3(full, empty, bubbles);
    }
    else if (intensity == 4) {
        ll full = opt<ll>("full");
        ll empty = opt<ll>("empty");
        ll bubbles = opt<ll>("bubbles");
        return Gen4(full, empty, bubbles);
    }
    else if (intensity == 5) {
        ll full = opt<ll>("full");
        ll empty = opt<ll>("empty");
        ll bubbles = opt<ll>("bubbles");
        return Gen5(full, empty, bubbles);
    }
    else if (intensity == 6) {
        ll full = opt<ll>("full");
        ll empty = opt<ll>("empty");
        ll bubbles = opt<ll>("bubbles");
        return Gen6(full, empty, bubbles);
    }
    else if (intensity == 7) {
        ll full = opt<ll>("full");
        ll empty = opt<ll>("empty");
        ll bubbles = opt<ll>("bubbles");
        ll crop = opt<ll>("crop");
        return Gen7(full, empty, bubbles, crop);
    }
/*    else if (intensity == 8) {
        ll full = opt<ll>("full");
        ll empty = opt<ll>("empty");
        ll bubbles = opt<ll>("bubbles");
        return Gen8(full, empty, bubbles);
    }*/
    else {
        std::cerr << "Unknown 'intensity' parameter = " << intensity << std::endl;
    }
    return 0;
}
