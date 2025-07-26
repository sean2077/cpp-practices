#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

/** 背包问题变种：零钱兑换 TC: O(N*A), SC: O(A) */
int coinChange(vector<int>& coins, int A) {
    int N = coins.size();
    vector<int> dp(A + 1, A + 1); // 这里用了个技巧，将初始值设为最大可能值+1，如果设 INT_MAX 的话后面会很麻烦
    dp[0] = 0;                    // 目标金额为0时不需要硬币
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= A; j++) {
            if (j >= coins[i - 1]) {
                dp[j] = min(dp[j], dp[j - coins[i - 1]] + 1);
            } else {
                // 新硬币比目标金额大，不用新硬币
                // dp[j] = dp[j];
            }
        }
    }
    return dp[A] == A + 1 ? -1 : dp[A];
}

/** 背包问题变种：零钱兑换II TC: O(N*A), SC: O(A) */
int coinChangeII(vector<int>& coins, int A) {
    int N = coins.size();
    vector<int> dp(A + 1, 0);
    dp[0] = 1; // 目标金额为0只有一种组合方式：空集
    for (int i = 1; i <= N; i++) {
        int coin = coins[i - 1];
        for (int j = 1; j <= A; j++) {
            if (coin <= j) {
                // 不选新硬币的方案 dp[j], 选新硬币的方案 dp[j-coin]
                dp[j] = dp[j] + dp[j - coin];
            } else {
                // 新硬币比目标金额大，肯定用不到
                // dp[j] = dp[j];
            }
        }
    }
    return dp[A];
}

int main() {
    // 零钱兑换
    vector<int> coins{1, 2, 5};
    cout << "coinChange: " << coinChange(coins, 11) << endl;
    cout << "coinChangeII: " << coinChangeII(coins, 5) << endl;

    return 0;
}
