#include <algorithm>
#include <vector>

using ll = long long;
using ld = long double;

// ll calc(std::vector<std::vector <ll>>& a);

ll CalculateRawScore(ll n, ll m, ll empty, const std::vector<std::vector<ll>>& state);

 ld CalculateScore(ll n, ll m, ll empty, const std::vector<std::vector<ll>>& state);

ll CalculateSquareScore(ll n, ll m, ll empty, const std::vector<std::vector<ll>>& state);

ld CalculateSquareScoreLd(ll n, ll m, ll empty, ll pscore, ll jscore);

// ll CalculateKScore(ll n, ll m, ll empty, const std::vector<std::vector<ll>>& state, ll k);

// ll CalculateMaxScore(ll n, ll m, ll empty, const std::vector<std::vector<ll>>& state);

// ld CalculateMaxScoreD(ll n, ll m, ll empty, const std::vector<std::vector<ll>>& state);