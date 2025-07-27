/*
输入

输入的第一行给出 T，即案例数量。对于每种情景，第一行输入一个浮点数 P，表示 Roy 需要低于该概率的概率，以及一个整数 N，表示他计划抢劫的银行数量。接下来是 N 行，其中第 j 行给出一个整数 Mj 和一个浮点数 Pj 。
银行 j 有 Mj 百万，抢劫被抓的概率为 Pj 。

输出

对于每个测试用例，输出一行，表示在被抓住的概率小于设定的极限的情况下，他可以期望获得的最大数百万数。

注意事项和约束
0 < T <= 100
0.0 <= P <= 1.0
0 < N <= 100
0 < Mj <= 100
0.0 <= Pj <= 1.0
如果银行被抢劫，它就会破产，你可以假设所有概率都是独立的，因为警察的资金非常低。

示例输入

3
0.04 3
1 0.02
2 0.03
3 0.05
0.06 3
2 0.03
2 0.03
3 0.05
0.10 3
1 0.03
2 0.02
3 0.05
示例输出

2
4
6
 */
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

/** 暴力解法: 回溯遍历,dfs */
int _solve_v1(vector<int>& m, vector<float>& p, float P, int i, float pp) {
    // pp: 被抓概率
    if (i < 0) {
        return 0;
    }
    // 不抢当前银行
    int no = _solve_v1(m, p, P, i - 1, pp);

    // 尝试抢当前银行
    float new_prob = 1.0f - (1.0f - pp) * (1.0f - p[i]);
    int yes = -1; // 表示无效方案

    if (new_prob < P) { // 只有概率允许时才抢
        yes = _solve_v1(m, p, P, i - 1, new_prob) + m[i];
    }

    return max(no, yes); // max会自动处理-1的情况
}

int solve_v1(vector<int>& m, vector<float>& p, float P) {
    return _solve_v1(m, p, P, m.size() - 1, 0.f);
}

/** 进阶思路：0-1背包枚举0~最大可能金额下最小被抓概率，后倒序遍历找出最大金额 */
int solve_v2(vector<int>& m, vector<float>& p, float P) {
    int M = accumulate(m.begin(), m.end(), 0);
    vector<float> dp(M + 1, 1.f); // 枚举抢指定金额情况下的被抓概率，1可以表示不可能的金额情况
    dp[0] = 0.f;                  // 不抢一定不被抓
    for (int i = 0; i < m.size(); i++) {
        for (int j = M; j > 0; j--) {
            if (j >= m[i]) {
                dp[j] = min(dp[j], 1 - (1 - dp[j - m[i]]) * (1 - p[i]));
            }
        }
    }
    int i = M;
    for (; i >= 0; i--) {
        if (dp[i] < P) {
            break;
        }
    }
    return i;
}

int main() {
    int T;
    cin >> T;
    while (T--) {
        float P;
        int N;
        cin >> P >> N;
        vector<int> m(N);
        vector<float> p(N);
        for (int i = 0; i < N; i++) {
            cin >> m[i] >> p[i];
        }
        int result = solve_v2(m, p, P);
        cout << result << endl;
    }
    return 0;
}
