/*
Description
开张的第二天，dzl 再次带着 m 元钱来找 ala，ala 从新拿出了他准备的 n 件物品，每件物品有一个实用值和对应的价格，dzl 想在 m 元钱内买到实用值最多的物品组合。这次由于 ala 拿出的物品比昨天多，所以 dzl 可以很轻松的计算出来。dzl 急着去找女朋友，就想让你帮他解决这个问题。
Input
数据的第一行有两个整数，分别为 n 件物品和 m 元钱 (1<=n<=1000，1<=m<=1000)
第 i+1 行为第 i 件物品的实用值 c[i] 和价格 w[i] 元 (1<=c[i],w[i]<=1000)
Output
输出一行为可以得到的最大的实用值。
Sample Input Copy
5 10
1 5
2 4
3 3
4 2
5 1
Sample Output Copy
14
*/
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

/** ala store: 0-1背包 */
int alaStoreII(vector<int>& c, vector<int>& w, int m) {
    int n = c.size();
    vector<int> dp(m + 1, 0);
    for (int i = 0; i < n; i++) {
        for (int j = m; j > 0; j--) {
            if (j >= w[i]) {
                dp[j] = max(dp[j], dp[j - w[i]] + c[i]);
            }
        }
    }
    return dp[m];
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<int> c(n), w(n);
    for (int i = 0; i < n; i++) {
        cin >> c[i] >> w[i];
    }
    cout << alaStoreII(c, w, m) << endl;
    return 0;
}
