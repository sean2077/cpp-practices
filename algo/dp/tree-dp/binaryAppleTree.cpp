/*
二叉苹果树 https://www.luogu.com.cn/problem/P2015
*/
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

struct edge {
    int to, w; // to: 连接的节点，w: 这条边上的苹果数
};

vector<edge> G[110];   // 邻接表存储树
int f[110][110], n, q; // f[i][j]: 以i为根的子树中保留j条边能获得的最大苹果数
bool vis[110];

/* TC: O(N × Q²) SC: O(N × Q) */
void dfs(int pos) {
    if (G[pos].size() == 1) {
        return;
    }
    int ch[2], w[2], len = 0;
    for (auto i : G[pos]) {
        if (!vis[i.to]) {
            w[len] = i.w;     // 记录到子节点的边权（苹果数）
            ch[len++] = i.to; // 记录子节点
        }
    }
    vis[pos] = 1; // 标记当前节点已访问
    dfs(ch[0]);
    dfs(ch[1]);
    for (int i = 1; i <= q; i++) {
        // 情况1：只选择一个子树
        f[pos][i] = max(f[ch[1]][i - 1] + w[1], f[ch[0]][i - 1] + w[0]);
        // 情况2：两个子树都选择
        for (int j = 0; j <= i - 2; j++) {
            f[pos][i] = max(f[pos][i], f[ch[0]][j] + w[0] + f[ch[1]][i - j - 2] + w[1]);
        }
    }
    vis[pos] = 0;
}

int main() {
    cin >> n >> q;
    for (int i = 1; i < n; i++) {
        int u, v;
        edge e;
        cin >> u >> v >> e.w;
        e.to = v, G[u].emplace_back(e);
        e.to = u, G[v].emplace_back(e);
    }
    dfs(1);
    cout << f[1][q];
    return 0;
}
