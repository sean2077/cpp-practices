/*
某公司有一批货物，系了 2 个轮船进行运输。每条轮船上可以运输不同容量的货物。由于 2 个轮船的发船时间不一样，同一个货物通过不同的轮船运输到终点后，利润也是不一样的，现在需要根据不同轮船的容量、货物的容量以及在不同轮船上进行运输的利润有效地分配货物，从而达到货物价值最大化。每个货物只有 1 件。
输入：
第一行输入为 A B M，A 代表轮船 A 的容量，B 代表轮船 B 的容量，M 代表货物的个数。
之后 M 行分别代表每个货物的容量、使用轮船 A 运输的利润、使用轮船 B 运输的利润。
输出：
最大利润。
例子：
输入：
5 6 4
4 3 2
3 1 4
2 2 4
2 3 3
输出：11
解释：轮船 A 运输货物 1 或者 4，利润 3；轮船 B 运输货物 2 和 3，利润 8；所以最大的利润是 11。
*/
#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

using namespace std;

/** 两个货物. TC: O(ABN) SC: O(AB) */
int twoCargo_dp(int A, int B, const vector<int>& capacities, const vector<int>& profitsA, const vector<int>& profitsB) {
    int N = capacities.size();
    vector<vector<int>> dp(A + 1, vector<int>(B + 1, 0)); // A 装 i，B 装 j 时的最大利润
    for (int i = 0; i < N; ++i) {
        for (int a = A; a > 0; --a) {
            for (int b = B; b > 0; --b) {
                if (a >= capacities[i] && b >= capacities[i]) { // 选A、B或不选
                    dp[a][b] = max(dp[a][b], max(dp[a - capacities[i]][b] + profitsA[i], dp[a][b - capacities[i]] + profitsB[i]));
                } else if (a >= capacities[i]) { // 选A或不选
                    dp[a][b] = max(dp[a][b], dp[a - capacities[i]][b] + profitsA[i]);
                } else if (b >= capacities[i]) { // 选B或不选
                    dp[a][b] = max(dp[a][b], dp[a][b - capacities[i]] + profitsB[i]);
                }
            }
        }
    }
    return dp[A][B];
}

#include "test_utils.hpp"

int main() {
    cout << "Two Cargo Problem" << endl;
    cout << "TestCase: (A, B, Capacities, ProfitsA, ProfitsB, Expected)" << endl;

    using Case = tuple<int, int, vector<int>, vector<int>, vector<int>, int>;
    vector<Case> cases = {
        make_tuple(5, 6, vector<int>{4, 3, 2, 2}, vector<int>{3, 1, 2, 3}, vector<int>{2, 4, 4, 3}, 11),
        // 可在此添加更多测试用例
    };

    vector<pair<string, decltype(&twoCargo_dp)>> methods = {
        {"twoCargo_dp", twoCargo_dp},
        // 可在此添加更多解法
    };

    runTests(methods, cases);
    return 0;
}
