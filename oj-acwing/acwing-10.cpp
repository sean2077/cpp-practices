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

// 尝试先插入根节点再遍历子节点树
void dfs2(int u, const vector<Item>& items, int V, vector<vector<int>>& dp) {
    // 将当前节点加入背包(因为不插 u 的话后续的子节点也不能选, 结果为 0, 所以能插必须插)
    for (int j = V; j >= items[u].volume; --j) {
        dp[u][j] = items[u].value;
    }

    // 遍历u的所有子节点, 从每个子节点树的所有组合中选择一个最优的方案(分组背包)
    for (int s : items[u].sons) {
        // 递归处理子节点, 确保子节点的 dp[s] 已经准备好
        dfs2(s, items, V, dp);

        // 分组背包合并
        for (int j = V; j > items[u].volume; --j) {
            // 针对每一个 dp[u][j], 我们需从子节点树的 dp[s] 分组背包中选择一个
            // 不过必须保证 j - k >= items[u].volume, 这样dp[u][V_u~V]才会带 u
            // 进而才能取子节点
            // 而 k 从 V_s 开始是因为当 k < V_s 时, 其连自己都拿不了, 其值为0, 选不选都一样
            for (int k = items[s].volume; j - k >= items[u].volume; ++k) { // 这里正序逆向都可
                dp[u][j] = max(dp[u][j], dp[u][j - k] + dp[s][k]);
            }
        }
        // 本质上这里就是现背包泛化物品和子节点泛化物品的求和
    }
}

// 递归树第二种写法
int knapsack_dep2(int V, const vector<int>& volumes, const vector<int>& values, const vector<int>& parents) {
    int N = volumes.size();
    // 构建树
    int root;
    vector<Item> items;
    items.reserve(N);
    for (int i = 0; i < N; ++i) {
        items.emplace_back(volumes[i], values[i], parents[i]);
        if (parents[i] == -1) {
            root = i; // 记录根节点
        }
    }
    // 连接子节点
    for (int i = 0; i < N; ++i) {
        if (items[i].parent != -1) {
            items[items[i].parent].sons.push_back(i);
        }
    }
    // DP数组 dp_{i,j} 表示以 i 为根节点的树中,背包体积j时取的最大价值
    vector<vector<int>> dp(N, vector<int>(V + 1, 0));
    // 深度优先遍历树
    dfs2(root, items, V, dp);
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
    int max_value = knapsack_dep2(V, volumes, values, parents);
    cout << max_value << endl;
    return 0;
}
