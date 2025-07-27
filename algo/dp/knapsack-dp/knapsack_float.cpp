/**
 * @file knapsack_float.cpp
 * @author sean2077 (sean2077@163.com)
 * @brief 0-1背包问题（浮点数）
 * @version 0.1
 * @date 2025-07-23
 *
 * @copyright Copyright (c) 2025
 *
 */
// > [!question]
// > 0-1 背包问题，一共 `n <= 20` 个物品，每个物品价格 `prices[i]` （浮点数），重量 `weights[i]` （浮点数），背包容量 `M` （浮点数）。求最大能装的价值是多少？
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

/** 0-1背包问题（浮点数）: 转成经典0-1背包问题（整数） TC: O(N*M) SC: O(M+2N) */

int knapsack01(const vector<int>& prices, const vector<int>& weights, int M) {
    int N = prices.size();
    vector<int> dp(M + 1, 0);
    for (int i = 1; i <= N; i++) {
        for (int j = M; j > 0; j--) {
            if (weights[i - 1] <= j) {
                dp[j] = max(dp[j], dp[j - weights[i - 1]] + prices[i - 1]);
            }
        }
    }
    return dp[M];
}

float knapsack01_float_to_int(const vector<float>& prices, const vector<float>& weights, float M) {
    int N = prices.size();
    // 将浮点数转换为整数，乘以100（或其他合适的倍数）来避免精度问题
    int multiplier = 100;

    vector<int> int_prices(N), int_weights(N);
    for (int i = 0; i < N; ++i) {
        int_prices[i] = static_cast<int>(prices[i] * multiplier + 0.5f); // 四舍五入到整数
        int_weights[i] = static_cast<int>(weights[i] * multiplier + 0.5f);
    }
    return knapsack01(int_prices, int_weights, static_cast<int>(M * multiplier + 0.5f)) / static_cast<float>(multiplier);
}

/** 0-1背包问题（浮点数）：二进制枚举 TC: O(N*2^N) SC: O(1) */
float knapsack01_float_binary_enum(const vector<float>& prices, const vector<float>& weights, float M) {
    int N = prices.size();
    float max_value = 0.f;
    uint32_t total = static_cast<uint32_t>(1) << N; // 用static_cast<uint32_t>(1) << N，避免移位溢出
    for (uint32_t mask = 0; mask < total; ++mask) {
        float current_weight = 0.f, current_value = 0.f;
        for (int i = 0; i < N; ++i) {
            if (mask & (1u << i)) {
                current_weight += weights[i];
                if (current_weight > M) {
                    break; // 剪枝：超重直接跳出
                }
                current_value += prices[i];
            }
        }
        if (current_weight <= M) { // 确保当前重量不超过背包容量
            max_value = max(max_value, current_value);
        }
    }
    return max_value;
}

/** 0-1背包问题（浮点数）：递归回溯 TC: O(2^N) SC: O(N)
 * 由于 M 是 float，无法直接用于哈希或数组下标，不适合常规记忆化（DP）。
 * 若需记忆化，需对 M 离散化（如四舍五入到某一精度），但会引入精度误差。
 */
float _knapsack01_float_recursive_helper(const vector<float>& prices, const vector<float>& weights, float M, int index) {
    if (index < 0 || M <= 0) {
        return 0.f; // 基本情况：没有物品或背包容量为0
    }
    // 不选当前物品
    float max_value = _knapsack01_float_recursive_helper(prices, weights, M, index - 1);
    // 选当前物品（如果不超重）
    if (weights[index] <= M) {
        max_value = max(max_value,
                        prices[index] + _knapsack01_float_recursive_helper(prices, weights, M - weights[index], index - 1));
    }
    return max_value;
}

float knapsack01_float_recursive(const vector<float>& prices, const vector<float>& weights, float M) {
    int N = prices.size();
    return _knapsack01_float_recursive_helper(prices, weights, M, N - 1);
}

/** 0-1背包问题（浮点数）：折半枚举 TC: O() SC: O() */
float knapsack01_float_half_enum(const vector<float>& prices, const vector<float>& weights, float M) {
    int N = prices.size(), N1 = N / 2, N2 = N - N1;
    float max_value = 0.f;

    // ⭐1: 对 weights 进行排序，按照重量从小到大排列，这样可以：
    // (1) 当组合重量超过 M，可以提前中断整个枚举循环（因为后续的组合重量一定会超过 M）
    // (2) 在枚举组合时，可以丢弃价值比前面低的组合（因为组合是按重量排序的，如果该组合价值比前低，重量却比前面高，是不可能选择的）
    // (3) 省去后续对枚举组合的排序操作
    vector<int> sorted_indices(N);
    std::iota(sorted_indices.begin(), sorted_indices.end(), 0);
    sort(sorted_indices.begin(), sorted_indices.end(), [&](int i, int j) { return weights[i] < weights[j]; });

    // 二进制枚举 TC: O((N/2)*2^(N/2))
    auto binary_enum = [&](int left, int right, vector<pair<float, float>>& wp) {
        wp.reserve(1u << (right - left + 1)); // 预留空间，避免频繁扩容
        for (uint32_t mask = 0; mask < (1u << (right - left + 1)); mask++) {
            float weight_sum = 0.f, value_sum = 0.f;
            for (int i = left; i <= right; i++) {
                if (mask & (1u << (i - left))) {
                    weight_sum += weights[sorted_indices[i]];
                    if (weight_sum > M) {
                        break; // 超重，跳出循环
                    }
                    value_sum += prices[sorted_indices[i]];
                }
            }
            if (weight_sum <= M) { // 确保当前重量不超过背包容量
                // ⭐1(2): 丢弃价值比前面低的组合（因为组合是按重量排序的，该组合重量比前面高，价值比前低，是不可能选择的）
                if (wp.empty() || wp.back().second < value_sum) {
                    wp.emplace_back(weight_sum, value_sum);
                }
            } else {
                break; //⭐1(1):提前中断整个枚举循环（因为后续的组合重量一定会超过 M）
            }
        }
        wp.shrink_to_fit(); // 收缩容量，避免浪费空间
    };

    // 枚举左右半部分的物品组合方式，丢弃超过背包容量的组合
    vector<pair<float, float>> wp1, wp2;
    binary_enum(0, N1 - 1, wp1);
    binary_enum(N1, N - 1, wp2);
    // ⭐ Note: wp2 本身可以不存储的，只要在遍历右边时合并即可，但会导致一些额外的计算，本质也是空间换时间，这里看如何取舍了

    // 合并左右半部分的组合，寻找最优解 TC: O(N log N)
    for (const auto& [w1, v1] : wp1) {
        // ⭐2: 二分法找到重量最接近 M - w1 的组合 TC: O(log N)
        auto it = lower_bound(wp2.begin(), wp2.end(), make_pair(M - w1, 0.f));
        if (it != wp2.begin()) {
            --it;
            max_value = max(max_value, v1 + it->second);
        } else {
            // 如果没有找到合适的组合，说明当前 w1 已经是最大的了
            max_value = max(max_value, v1);
        }
    }

    return max_value;
}

#include "test_utils.hpp"

int main() {
    cout << "0-1 Knapsack Problem (Float)" << endl;
    cout << "TestCase: (Prices, Weights, M, Wanted)" << endl;

    // 测试数据
    using TestCase = tuple<vector<float>, vector<float>, float, float>;
    vector<TestCase> test_cases{
        {{51.56f, 23.56f, 45.62f, 42.23f, 65.32f, 32.45f, 45.78f, 98.32f, 45.62f, 95.32f,
          99.45f, 22.48f, 51.56f, 23.56f, 45.62f, 42.23f, 65.32f, 32.45f, 45.78f, 98.32f},
         {23.56f, 31.45f, 62.54f, 15.32f, 12.32f, 65.12f, 15.65f, 62.15f, 32.15f, 15.44f,
          45.65f, 32.15f, 23.56f, 31.45f, 62.54f, 15.32f, 12.32f, 65.12f, 15.65f, 62.15f},
         678.91f, 1050.07f},
    };
    vector<pair<string, decltype(&knapsack01_float_to_int)>> methods{
        {"knapsack01_float_to_int",      knapsack01_float_to_int     },
        {"knapsack01_float_binary_enum", knapsack01_float_binary_enum},
        {"knapsack01_float_recursive",   knapsack01_float_recursive  },
        {"knapsack01_float_half_enum",   knapsack01_float_half_enum  },
    };

    cout << fixed << setprecision(4); // 设置输出格式为小数点后两位

    runTests(methods, test_cases);

    return 0;
}
