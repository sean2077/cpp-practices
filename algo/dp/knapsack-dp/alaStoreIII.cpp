/*
Description
开张的第三天，dzl 气势汹汹地带着 m 元钱来找 ala，ala 这次不打算为难他，拿出的 n 种物品中每种物品有一个对应的价格，dzl 想知道花光 m 元钱可以购买的物品方案总数。今天可算是难住 dzl 了，想让你再次帮他解决这个问题。
Input
数据为单组
数据的第一行有两个整数，分别为 n 件物品和 m 元钱 (1<=n<=200，1<=m<=200)
第 i 行为第 i 种物品的价格 w[i] 元 (1<=w[i]<=200)
数据保证没有两种物品价格重复
Output
输出一行为可以购买的方案总数
Sample Input Copy
3 5
1
2
3
Sample Output Copy
5
HINT
样例中可以购买的方案分别为
[2,3]，[1,1,3]，[1,1,1,2]，[1,2,2]，[1,1,1,1,1]。
*/
#include <iostream>
#include <vector>
using namespace std;

int alaStoreIII(vector<int>& w, int m) {
    // 完全背包问题，枚举0~m元对应的最大方案数
    int n = w.size();
    vector<int> dp(m + 1, 0);
    dp[0] = 1;
    for (int i = 0; i < n; i++) {
        for (int j = 1; j <= m; j++) {
            if (j >= w[i]) {
                dp[j] = dp[j] + dp[j - w[i]];
            }
        }
    }
    return dp[m];
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<int> w(n);
    for (int i = 0; i < n; i++) {
        cin >> w[i];
    }
    cout << alaStoreIII(w, m) << endl;
    return 0;
}
