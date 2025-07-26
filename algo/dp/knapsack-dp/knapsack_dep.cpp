/*
有 N 个物品和一个容量是 V 的背包。

物品之间具有依赖关系，且依赖关系组成一棵树的形状。如果选择一个物品，则必须选择它的父节点。

每件物品的编号是 i，体积是 vi，价值是 wi，父节点序号是 pi。根节点为 -1。

求解将哪些物品装入背包，可使物品总体积不超过背包容量，且总价值最大。

输出最大价值。
*/
#include <chrono>
#include <iostream>
#include <vector>

#include "print_utils.hpp"
using namespace std;

struct Item {
    int volume;
    int value;
    int parent;       // 父节点索引
    vector<int> sons; // 子节点索引

    Item(int vol, int val, int p) : volume(vol), value(val), parent(p) {}
};

void dfs(int u, const vector<Item>& items, int V, vector<vector<int>>& dp) {
    // dp_{i,j} 表示以 i 为根节点的树中,背包体积j时取的最大价值

    // 遍历u的所有子节点, 从每个子节点树的所有组合中选择一个最优的方案(分组背包)
    for (int s : items[u].sons) {
        // 递归处理子节点, 确保子节点的 dp[s] 已经准备好
        dfs(s, items, V, dp);

        // 分组背包合并
        // 解释: 因为子节点树 s 的所有可能的分支 dp[s][0]~dp[s][M] 中只能选择一种, 即分组背包
        // 我们的目的是为了更新 dp[u][0~(V-V_u)] 的状态, 为什么右边界是 V-V_u 呢?
        // 因为后续还要将 u 物品加入背包, 此处 dp[u][0~V-V_u] 仅考虑拿子节点树的分支, 后续再插入 u
        // 就刚好等于放 u 和每个子节点树的最优组合, 这里更新 dp[u][(V-V_u)~V] 是没有意义的,
        // 因为后续也不会取这种状态.
        for (int j = V - items[u].volume; j >= 0; --j) { // 需要压缩 dp[u] 空间, 这里必须倒序
            // 针对每一个 dp[u][j], 我们需从子节点树的 dp[s][0]~dp[s][j] 的组合中选择一个
            for (int k = 0; k <= j; ++k) { // 这里正序逆向都可
                dp[u][j] = max(dp[u][j], dp[u][j - k] + dp[s][k]);
            }
        }
        // 至此, 我们完成了分组背包(s)合并到父背包(u)的过程
    }
    //至此, 我们完成所以子节点树的分组背包合并到父节点的过程, 注意此时的 dp[u][0~V] 还不是最终状态,
    // 因为当前的 dp[u][0~(V-V_u)] 代表的物品组合中还没有当前节点 u 的物品, dp[u][(V-V_u)~V] 还没有被更新
    // 所以下面就是更新 dp[u][0~V] 的过程

    // 将当前节点加入背包(因为不插 u 的话结果为 0, 所以能插必须插)
    for (int j = V; j >= items[u].volume; --j) {
        dp[u][j] = dp[u][j - items[u].volume] + items[u].value;
    }
    // 当前节点的物品体积大于背包体积不可能被选中
    // 这一段不能省略, 因为前面更新 dp[u][0~(V-V_u)] 时, 本质是更新 dp[u][V_u~V] 的状态, 这些状态中存在非0的情况,
    // 所以在这要将 dp[u][0~(V_u-1)] 的状态清零, 以免影响最终结果
    for (int j = 0; j < items[u].volume; ++j) {
        dp[u][j] = 0;
    }
}

// 尝试先插入根节点再遍历子节点树
void dfs2(int u, const vector<Item>& items, int V, vector<vector<int>>& dp) {
    // 将当前节点加入背包(因为不插 u 的话后续的子节点也不能选, 结果为 0, 所以能插必须插)
    for (int j = V; j >= items[u].volume; --j) {
        dp[u][j] = items[u].value;
    }
    // 至此,dp[u][V_u~V] 都是带 u 物品的状态, 其他状态都为 0

    // 遍历u的所有子节点, 从每个子节点树的所有组合中选择一个最优的方案(分组背包)
    for (int s : items[u].sons) {
        // 递归处理子节点, 确保子节点的 dp[s] 已经准备好
        dfs(s, items, V, dp);

        // 分组背包合并
        for (int j = V; j >= items[u].volume; --j) {
            // 针对每一个 dp[u][j], 我们需从子节点树的 dp[s] 分组背包中选择一个
            // 不过必须保证 j - k >= items[u].volume, 这样dp[u][V_u~V]才会带 u
            // 进而才能取子节点
            // 而 k 从 V_s 开始是因为当 k < V_s 时, 其连自己都拿不了, 其值为0, 选不选都一样
            for (int k = items[s].volume; j - k >= items[u].volume; ++k) { // 这里正序逆向都可
                dp[u][j] = max(dp[u][j], dp[u][j - k] + dp[s][k]);
            }
        }
    }
}

/* 背包问题（带依赖）: TC:O(N * V^2) SC: O(N * V) */
int knapsack_dep(int V, const vector<int>& volumes, const vector<int>& values, const vector<int>& parents) {
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
    dfs(root, items, V, dp);
    // 返回最大价值
    return dp[root][V];
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

int main() {
    cout << "Knapsack with Dependencies" << endl;

    // 测试用例：
    int V = 7;
    vector<int> volumes = {2, 2, 3, 4, 3};
    vector<int> values = {3, 2, 5, 7, 6};
    vector<int> parents = {-1, 0, 0, 1, 1};
    // int V = 5;
    // vector<int> volumes = {1, 2, 2, 2, 2};
    // vector<int> values = {1, 10, 10, 100, 100};
    // vector<int> parents = {-1, 0, 0, 1, 2};
    // int V = 7;
    // vector<int> volumes = {1, 2, 2, 2};
    // vector<int> values = {1, 10, 20, 30};
    // vector<int> parents = {-1, 0, 0, 0}; // 0为根，1/2/3都是0的子节点

    // result: 11

    cout << "V: " << V << endl;
    cout << "Volumes: " << volumes << endl;
    cout << "Values: " << values << endl;
    cout << "Parents: " << parents << endl;
    cout << "------" << endl;

    auto test = [&](const string& method_name, int (*method)(int, const vector<int>&, const vector<int>&, const vector<int>&)) {
        auto start = chrono::high_resolution_clock::now();
        int max_value = method(V, volumes, values, parents);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<float, milli> duration = end - start;
        cout << method_name << ": " << max_value << " (Time: " << duration.count() << " ms)" << endl;
    };

    test("knapsack_dep", knapsack_dep);
    test("knapsack_dep2", knapsack_dep2);

    return 0;
}
