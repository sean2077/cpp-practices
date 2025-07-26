/**
 * @file knapsack.cpp
 * @author sean2077 (sean2077@163.com)
 * @brief 背包问题及其变种问题
 * @version 0.1
 * @date 2025-07-23
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

/* 0-1 背包：暴力搜索 TC: O(2^n), SC: O(n) */

int _knapsack01_dfs_helper(vector<int>& wgt, vector<int>& val, int i, int c) {
    // 无物品可放或背包无容量
    if (i == 0 || c == 0) {
        return 0;
    }
    // 如果物品超过背包容量，则只能选择不放入
    if (wgt[i - 1] > c) {
        return _knapsack01_dfs_helper(wgt, val, i - 1, c);
    }

    // 放入该物品
    int v1 = _knapsack01_dfs_helper(wgt, val, i - 1, c - wgt[i - 1]) + val[i - 1];
    // 不放入该物品
    int v2 = _knapsack01_dfs_helper(wgt, val, i - 1, c);

    return max(v1, v2);
}

int knapsack01_dfs(vector<int>& wgt, vector<int>& val, int c) {
    return _knapsack01_dfs_helper(wgt, val, wgt.size(), c);
}

/** 01背包：记忆化搜索，TC: O(n*c), SC: O(n*c) */

int _knapsack01_dfs_mem_helper(vector<int>& wgt, vector<int>& val, vector<vector<int>>& mem, int i, int c) {
    // 无物品或背包无容量
    if (i == 0 || c == 0) {
        mem[i][c] = 0;
        return 0;
    }
    // 记忆化
    if (mem[i][c] != -1) {
        return mem[i][c];
    }

    // 如果物品超过背包容量，则只能选择不放入
    if (wgt[i - 1] > c) {
        mem[i][c] = _knapsack01_dfs_mem_helper(wgt, val, mem, i - 1, c);
        return mem[i][c];
    }

    // 放入该物品
    int v1 = _knapsack01_dfs_mem_helper(wgt, val, mem, i - 1, c - wgt[i - 1]) + val[i - 1];
    // 不放入该物品
    int v2 = _knapsack01_dfs_mem_helper(wgt, val, mem, i - 1, c);

    mem[i][c] = max(v1, v2);
    return mem[i][c];
}

int knapsack01_dfs_mem(vector<int>& wgt, vector<int>& val, int c) {
    int n = wgt.size();
    vector<vector<int>> mem(n + 1, vector<int>(c + 1, -1));
    return _knapsack01_dfs_mem_helper(wgt, val, mem, n, c);
}

/** 01背包：动态规划 TC: O() SC: O() */
int knapsack01_dp_v0(vector<int>& wgt, vector<int>& val, int c) {
    int n = wgt.size();
    vector<vector<int>> dp(n + 1, vector<int>(c + 1, 0));
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= c; ++j) {
            if (j < wgt[i - 1]) {
                dp[i][j] = dp[i - 1][j];
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - wgt[i - 1]] + val[i - 1]);
            }
        }
    }
    return dp[n][c];
}

/** 01背包：动态规划(空间进优化版,最优解法) TC: O(nc) SC: O(c) */
int knapsack01_dp(vector<int>& wgt, vector<int>& val, int c) {
    int n = wgt.size();
    vector<int> dp(c + 1, 0);
    for (int i = 1; i <= n; i++) {
        for (int j = c; j >= wgt[i - 1]; j--) {
            dp[j] = max(dp[j], dp[j - wgt[i - 1]] + val[i - 1]);
        }
    }
    return dp[c];
}

/** 完全背包 TC: O(nc) SC: O(c) */
int knapsack_dp(vector<int>& wgt, vector<int>& val, int c) {
    int n = wgt.size();
    vector<int> dp(c + 1, 0);
    for (int i = 1; i <= n; i++) {
        for (int j = wgt[i - 1]; j <= c; j++) {
            dp[j] = max(dp[j], dp[j - wgt[i - 1]] + val[i - 1]);
        }
    }
    return dp[c];
}

int main() {
    // 背包问题
    vector<int> wgt{10, 20, 30, 40, 50};
    vector<int> val{50, 120, 150, 210, 240};
    cout << "knapsack01_dfs: " << knapsack01_dfs(wgt, val, 50) << endl;
    cout << "knapsack01_dfs_mem: " << knapsack01_dfs_mem(wgt, val, 50) << endl;
    cout << "knapsack01_dp_v0: " << knapsack01_dp_v0(wgt, val, 50) << endl;
    cout << "knapsack01_dp (sota): " << knapsack01_dp(wgt, val, 50) << endl;
    cout << "knapsack_dp: " << knapsack_dp(wgt, val, 50) << endl;

    return 0;
}
