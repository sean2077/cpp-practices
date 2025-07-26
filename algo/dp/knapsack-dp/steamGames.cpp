/**
Steam 上有许多游戏和 DLC(游戏拓展内容)。每个游戏和 DLC 都有其价格和耐玩值，购买 DLC 必须先购买对应的游戏主体, 购买游戏主体则不一定要买其 DLC, 每个游戏可以有多个 DLC。

给定一个游戏 DLC 清单和一个预算，你可以从清单上选择购买游戏和 DLC，每种只能买一份, 请你设计一个算法，求出在不超预算的情况下能获取的最大化耐玩值。

### 输入描述

* 一个整数 `M` (1 ≤ M ≤ 10^4)，表示你可用的预算。
- `vector<int> prices`：一个整数数组，表示游戏或 DLC 的价格, 1 ≤ prices[i] ≤ 10^3。
- `vector<int> values`：一个整数数组，表示游戏或 DCL 的耐玩值, 1 ≤ values[i] ≤ 100。
- `vector<int> indices`：一个整数数组，表示游戏或 DLC 对应的编号，游戏本体的编号为 -1, DLC 的编号为其游戏本体在数组中的序号。

### 输出描述

* 输出一个整数，表示在不超过预算 `M` 元的前提下，能获取的最大耐玩值。
 */
#include <algorithm>
#include <chrono>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "print_utils.hpp"

using namespace std;

int steamGames_dp(const vector<int>& prices, const vector<int>& values, const vector<int>& indices, int M) {
    int N = prices.size();
    unordered_map<int, vector<int>> dlc_id_map;
    for (int i = 0; i < N; ++i) {
        if (indices[i] != -1) {
            dlc_id_map[indices[i]].emplace_back(i);
        }
    }
    vector<int> dp(M + 1, 0); // 枚举每个金额下能买到的最大耐玩值
    for (int i = 0; i < N; ++i) {
        // 跳过非游戏主体的物品，游戏DLC放在其主体那轮循环一起讨论
        if (indices[i] != -1) {
            continue;
        }
        // 对 DLC 按价格从低到高排序，便于后续提前中断枚举
        auto& dlc_ids = dlc_id_map[i];
        int dlc_cnt = dlc_ids.size();
        sort(dlc_ids.begin(), dlc_ids.end(), [&](int l, int r) { return prices[l] < prices[r]; });

        for (int j = M; j >= prices[i]; --j) {
            // 枚举DLC的所有选购组合，可以用二进制枚举，也可以dfs递归枚举
            // 二进制枚举直观简单，但性能可能不如递归
            for (uint32_t mask = 0; mask < (1u << dlc_cnt); ++mask) {
                int sum_price = prices[i], sum_value = values[i]; // 捆绑购买主体
                for (int d = 0; d < dlc_cnt; ++d) {
                    if (mask & (1 << d)) {
                        sum_price += prices[dlc_ids[d]];
                        sum_value += values[dlc_ids[d]];
                    }
                }
                if (sum_price <= j) {
                    dp[j] = max(dp[j], dp[j - sum_price] + sum_value);
                } else {
                    break; // 游戏及DLC总价超过当前预算，中断枚举
                }
            }
        }
    }
    return dp[M];
}

// 分组背包合并 SC: O(MN+n*M^2)
int steamGames_grouped_dp(const vector<int>& prices, const vector<int>& values, const vector<int>& indices, int M) {
    int N = prices.size();
    unordered_map<int, vector<int>> dlc_id_map;
    for (int i = 0; i < N; ++i) {
        if (indices[i] != -1) {
            dlc_id_map[indices[i]].emplace_back(i);
        }
    }
    vector<int> dp(M + 1, 0);
    for (int i = 0; i < N; ++i) {
        if (indices[i] != -1) {
            continue; // 只处理主件
        }
        // 组内0-1背包：主件必须买，DLC可选
        vector<pair<int, int>> group_items; // (price, value)
        group_items.emplace_back(prices[i], values[i]);
        for (int k : dlc_id_map[i]) {
            group_items.emplace_back(prices[k], values[k]);
        }
        int group_size = group_items.size();
        // 组内dp，f[x]=主件+部分DLC总价为x时的最大耐玩值
        vector<int> f(M + 1, -1);
        f[group_items[0].first] = group_items[0].second; // 必须买主件
        for (int j = 1; j < group_size; ++j) {
            int p = group_items[j].first, v = group_items[j].second;
            for (int b = M; b >= p; --b) {
                if (f[b - p] != -1) {
                    f[b] = max(f[b], f[b - p] + v);
                }
            }
        }
        // 分组背包合并
        for (int j = M; j >= prices[i]; --j) {
            for (int k = prices[i]; k <= j; ++k) {
                if (f[k] != -1) {
                    dp[j] = max(dp[j], dp[j - k] + f[k]);
                }
            }
        }
    }
    return dp[M];
}

int main() {
    cout << "Steam Games Problem" << endl;

    // 测试用例：
    int M = 200;
    vector<int> prices = {100, 23, 59, 69, 30, 40};
    vector<int> values = {4, 3, 5, 2, 6, 7};
    vector<int> indices = {-1, 0, 0, -1, 3, -1};
    // result: 16

    cout << "M: " << M << endl;
    cout << "Prices: " << prices << endl;
    cout << "Values: " << values << endl;
    cout << "Indices: " << indices << endl;
    cout << "------" << endl;

    auto test = [&](const string& method_name, int (*method)(const vector<int>&, const vector<int>&, const vector<int>&, int)) {
        // 计时
        auto start = chrono::high_resolution_clock::now();
        int max_value = method(prices, values, indices, M);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<float, milli> duration = end - start;
        cout << method_name << ": " << max_value << " (Time: " << duration.count() << " ms)" << endl;
    };

    test("steamGames_dp", steamGames_dp);
    test("steamGames_grouped_dp", steamGames_grouped_dp);

    return 0;
}
