/*
你有 $n$ 个物品，每个物品重量为 $w_i$，价值为 $v_i$，数量为 $k_i$。你有一个承重上限为 $W$ 的背包，现在要求你在不超过重量上限的情况下选取价值和尽可能大的物品放入背包。求最大价值。
*/
#include <deque>
#include <iostream>
#include <vector>

using namespace std;

/* 朴素解法1: 构建新物品数组，转0-1背包 */
int knapsack_multi_naive1(const vector<int>& w, const vector<int>& v, const vector<int>& k, int W) {
    vector<int> new_w;
    vector<int> new_v;
    for (int i = 0; i < w.size(); ++i) {
        for (int j = 0; j < k[i]; ++j) {
            new_w.push_back(w[i]);
            new_v.push_back(v[i]);
        }
    }
    // 0-1背包问题
    int N = new_w.size();
    vector<int> dp(W + 1, 0);
    for (int i = 0; i < N; ++i) {
        for (int j = W; j >= new_w[i]; --j) {
            dp[j] = max(dp[j], dp[j - new_w[i]] + new_v[i]);
        }
    }
    return dp[W];
}

/* 朴素解法2：加物品循环 */
int knapsack_multi_naive2(const vector<int>& w, const vector<int>& v, const vector<int>& k, int W) {
    int N = w.size();
    vector<int> dp(W + 1, 0);
    for (int i = 0; i < N; ++i) {
        for (int j = W; j >= w[i]; --j) {
            for (int m = 1; m <= k[i] && m * w[i] <= j; ++m) {
                dp[j] = max(dp[j], dp[j - m * w[i]] + m * v[i]);
            }
        }
    }
    return dp[W];
}

/* 朴素解法3: 0-1背包 + 完全背包 */
int knapsack_multi_naive3(const vector<int>& w, const vector<int>& v, const vector<int>& k, int W) {
    int N = w.size();
    vector<int> dp(W + 1, 0);
    for (int i = 0; i < N; ++i) {
        if (k[i] * w[i] >= W) { // 物品足够多，可以认为可以无限拿 -> 完全背包
            for (int j = w[i]; j <= W; ++j) {
                dp[j] = max(dp[j], dp[j - w[i]] + v[i]);
            }
        } else { // 物品不够多 -> 0-1背包处理
            for (int j = W; j >= w[i]; --j) {
                // 物品循环
                for (int m = 1; m <= k[i] && m * w[i] <= j; ++m) {
                    dp[j] = max(dp[j], dp[j - m * w[i]] + m * v[i]);
                }
            }
        }
    }
    return dp[W];
}

/* 二进制优化 */
int knapsack_multi_binary(const vector<int>& w, const vector<int>& v, const vector<int>& k, int W) {
    // 构建二进制分组数组
    vector<int> new_w, new_v;
    new_w.reserve(w.size() * 20); // 这里的20根据k的取值范围调整
    new_v.reserve(v.size() * 20);
    for (int i = 0; i < (int)w.size(); i++) {
        int b = 1, c = k[i];
        while (c > b) {
            new_w.push_back(b * w[i]);
            new_v.push_back(b * v[i]);
            c -= b;
            b *= 2;
        }
        new_w.push_back(c * w[i]);
        new_v.push_back(c * v[i]);
    }

    // 0-1b背包
    int N = new_w.size();
    vector<int> dp(W + 1, 0);
    for (int i = 0; i < N; ++i) {
        for (int j = W; j >= new_w[i]; --j) {
            dp[j] = max(dp[j], dp[j - new_w[i]] + new_v[i]);
        }
    }
    return dp[W];
}

/* 单调队列优化 */
int knapsack_multi_monotonic_queue(const vector<int>& w, const vector<int>& v, const vector<int>& k, int W) {
    int N = w.size();
    vector<vector<int>> dp(2, vector<int>(W + 1, 0));
    int pre = 0, cur = 1;
    for (int i = 0; i < N; ++i) {
        // 按 y 分组
        for (int y = 0; y < w[i]; ++y) {
            // 朴素解法的转移方程为：
            //		f_{i,j} = max(f_{i-1,j-k*w[i]}+k*v[i]), k=0,1,...,k[i]
            // 令：
            //		g_{x,y} = f_{i,x*w[i]+y}, g'_{x,y} = f_{i-1,x*w[i]+y}, 其中 0<=y<w[i]
            // 则转移方程可表示为：
            //		g_{x,y} = f_{i,x*w[i]+y} = max(f_{i-1,x*w[i]+y-k*w[i]}+k*v[i]), k=0,1,...,k[i]
            //				= max(f_{i-1,(x-k)*w[i]+y}+k*v[i]), k=0,1,...,k[i]
            // 				= max(g'_{x-k,y}+k*v[i]), k=0,1,...,k[i]
            // 令：
            //		G_{x,y} = g'_{x,y} - x*v[i]
            // 则：
            //		g_{x,y} = max(g'_{x-k,y}+k*v[i]), k=0,1,...,k[i]
            // 				= max(G{x-k,y}+(x-k)*v[i]+k*v[i]), k=0,1,...,k[i]
            // 				= max(G{x-k,y}+x*v[i]), k=0,1,...,k[i]
            // 				= max(G{x-k,y}) + x*v[i] , k=0,1,...,k[i]
            // 其中 max 部分可以通过单调队列遍历，使复杂度由 O(k[i]) 降为 O(1)
            // 因此可以对序列: G_{x,y}, x=0,1,2,...,W/w[i] 用大小为 k[i] 的单调递减队列遍历
            // 本质上遍历的是：
            // 		G_{x,y} = f_{i-1,x*w[i]+y} - x*v[i] , x=0,1,2,...,W/w[i]
            // 则队列的左边界为 x-k[i], 右边界为 x, 小于左边界的值应从队首弹出
            // 设队首为 x_front, 则:
            // 		G_{x_front,y} = max(G{x-k,y}), k=0,1,...,k[i]
            // 所以要更新的状态量为：
            // 		f_{i,x*w[i]+y} = g_{x,y} = max(G_{x,y}, G_{x-1,y},...,G_{x-k[i],y}) + x*v[i]
            // 					   = max(G{x-k,y}) + x*v[i] , k=0,1,...,k[i]
            // 					   = f_{i-1,x_front*w[i]+y} - x_front*v[i] + x*v[i]
            deque<int> qu;
            for (int x = 0; x * w[i] + y <= W; ++x) {
                // 弹出不在范围的元素
                while (!qu.empty() && qu.front() < x - k[i]) {
                    qu.pop_front();
                }
                // 保持单调递减
                while (!qu.empty() && dp[pre][qu.back() * w[i] + y] - qu.back() * v[i] < dp[pre][x * w[i] + y] - x * v[i]) {
                    qu.pop_back();
                }
                qu.push_back(x);
                // 此时队首为最大
                dp[cur][x * w[i] + y] = dp[pre][qu.front() * w[i] + y] - qu.front() * v[i] + x * v[i];
            }
        }
        swap(pre, cur); // 注意: 因为这里从小到大遍历，不能像0-1背包那样从大到小，所以必须申请副本存i-1状态的，不然会被影响

        // 至此,经过 y,x 循环更新完 f_{i,0} ~ f_{i,W}
    }
    return dp[pre][W];
}

/* 单调队列优化(倒序遍历版) */
int knapsack_multi_monotonic_queue_reverse(const vector<int>& w, const vector<int>& v, const vector<int>& k, int W) {
    int N = w.size();
    vector<int> dp(W + 1, 0);
    for (int i = 0; i < N; ++i) {
        // 按 y 分组
        for (int y = 0; y < w[i]; ++y) {
            deque<int> qu;
            int r = (W - y) / w[i]; // 最大 x 值
            // 对于 x, 需要的窗口是 [x-k[i], x]，因此预先插入 [r-k[i]+1, r] 的元素
            for (int x = r; x > r - k[i] && x >= 0; --x) {
                // 此处元素肯定未出边界,因此不用验证
                // 保持单调递减, 新值放队首
                while (!qu.empty() && dp[qu.front() * w[i] + y] - qu.front() * v[i] < dp[x * w[i] + y] - x * v[i]) {
                    qu.pop_front();
                }
                qu.push_front(x);
            }
            for (int x = r; x >= 0; --x) {
                // 弹出不在范围的元素
                while (!qu.empty() && qu.back() > x) {
                    qu.pop_back();
                }
                // 对应要插入的新值为 x - k[i], 如果 x - k[i] < 0, 则不需要插入
                int new_x = x - k[i];
                if (new_x >= 0) {
                    // 保持单调递减, 新值放队首
                    while (!qu.empty() && dp[qu.front() * w[i] + y] - qu.front() * v[i] < dp[new_x * w[i] + y] - new_x * v[i]) {
                        qu.pop_front();
                    }
                    qu.push_front(new_x);
                }
                // 此时队尾为最大
                dp[x * w[i] + y] = dp[qu.back() * w[i] + y] - qu.back() * v[i] + x * v[i];
            }
        }
    }
    return dp[W];
}

#include "test_utils.hpp"

int main() {
    using TestCase = tuple<vector<int>, vector<int>, vector<int>, int, int>;
    vector<TestCase> test_cases{
        // {w, v, k, W, wanted}
        {{15, 8, 12, 6, 25, 18, 9, 14, 22, 11, 7, 16, 20, 13, 10, 19, 24, 17, 21, 26},
         {24, 15, 20, 12, 35, 28, 18, 25, 40, 22, 14, 30, 38, 26, 19, 33, 45, 31, 42, 50},
         {1500, 2800, 1200, 3500, 800, 1100, 3200, 1600, 700, 2200, 4000, 1300, 900, 2500, 3800, 1000, 600, 1800, 1400, 750},
         150000, 300000},
        // 可添加更多测试用例
    };
    vector<pair<string, decltype(&knapsack_multi_naive1)>> methods{
        {"朴素解法1",              knapsack_multi_naive1                 },
        {"朴素解法2",              knapsack_multi_naive2                 },
        {"朴素解法3",              knapsack_multi_naive3                 },
        {"二进制优化",             knapsack_multi_binary                 },
        {"单调队列优化",           knapsack_multi_monotonic_queue        },
        {"单调队列优化(倒序遍历)", knapsack_multi_monotonic_queue_reverse},
    };

    runTests(methods, test_cases);

    return 0;
}
