/*
10. 有依赖的背包问题 https://www.acwing.com/problem/content/description/10/
*/
#include <iostream>
#include <vector>

using namespace std;

struct Item {
    int volume;
    int value;
    int parent;       // 父节点索引
    vector<int> sons; // 子节点索引

    Item(int vol, int val, int p) : volume(vol), value(val), parent(p) {}
};

// 泛化物品求并

void dfs3(int u, const vector<Item>& items, int V, int v, vector<vector<int>>& dp) {
    if (v > V) {
        return;
    }

    // DP数组 dp_{i,j} 表示遍历到节点 i 时, 从已遍历的节点取体积不超过 j 且必选 i 时取的最大价值

    for (int s : items[u].sons) {
        // 遍历节点 s,
        for (int j = v + items[s].volume; j <= V; ++j) {
            dp[s][j] = dp[u][j - items[s].volume] + items[s].value;
        }

        // 递归处理s树, 遍历完全部 s 树的全部节点
        dfs3(s, items, V, v + items[s].volume, dp);
        // 至此, 除了u之前遍历完的点(含u, 记为Nu), 还包括s树的全部点(记为N's), s 树的全部点对应的 dp 都已经准备好了
        // 而 dp[u] 还是之前遍历完的点的状态, 需要更新
        for (int j = v + items[s].volume; j <= V; ++j) {
            // dp[u][j] 表示从Nu+N's中取u不取s且体积不超过j的解, 因为不取s就不能取s的子树节点 所以能这么表示
            // dp[s][j] 表示从Nu+N's中取u且取s的子树节点, 体积不超过j的解
            // dp[u][j] 和 dp[s][j] 不可能同时发生，故这里求并，得到的 dp[u][j] 是从Nu+N's中取u且体积不超过j的解
            // 也即 dp 的含义， 即 dp[u]得到更新
            dp[u][j] = max(dp[u][j], dp[s][j]);
        }
    }
}

int knapsack_dep3(int V, const vector<int>& volumes, const vector<int>& values, const vector<int>& parents) {
    int N = volumes.size();
    int root;
    vector<Item> items;
    items.reserve(N);
    for (int i = 0; i < N; ++i) {
        items.emplace_back(volumes[i], values[i], parents[i]);
        if (parents[i] == -1) {
            root = i; // 记录根节点
        }
    }
    for (int i = 0; i < N; ++i) {
        if (items[i].parent != -1) {
            items[items[i].parent].sons.push_back(i);
        }
    }

    // DP数组 dp_{i,j} 表示遍历到节点 i 时, 从已遍历的节点取体积不超过 j 且必选 i 时取的最大价值
    vector<vector<int>> dp(N, vector<int>(V + 1, 0));
    for (int j = items[root].volume; j <= V; ++j) {
        dp[root][j] = items[root].value; // 初始化根节点的状态
    }
    // 深度优先遍历树
    dfs3(root, items, V, items[root].volume, dp);
    // 返回最大价值
    return dp[root][V];
}

/*
输入样例
5 7
2 3 -1
2 2 1
3 5 1
4 7 2
3 6 2
输出样例：
11
*/
int main() {
    int N, V;
    cin >> N >> V;
    vector<int> volumes(N), values(N), parents(N);
    for (int i = 0; i < N; ++i) {
        cin >> volumes[i] >> values[i] >> parents[i];
        if (parents[i] != -1) {
            // 转换为0-indexed
            parents[i]--; // 这里假设输入的父节点是1-indexed
        }
    }
    // 调用背包函数
    int max_value = knapsack_dep3(V, volumes, values, parents);
    cout << max_value << endl;
    return 0;
}
