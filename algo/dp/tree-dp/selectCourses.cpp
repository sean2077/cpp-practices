/*
P2014 [CTSC1997] 选课 https://www.luogu.com.cn/problem/P2014
*/
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

struct Node {
    int score;  // 学分
    int parent; // 直接先修课的编号
    vector<int> sons;

    // 在树中的以本节点为根的树大小, 为部分解法添加
    int siz = 1;

    // 在二叉树中的左右子节点, 为二叉树解法添加
    int left = -1, right = -1; // -1 表示没有节点

    Node(int s, int p) : score(s), parent(p) {}
};

// 解法1: 分组背包

// u - 当前节点
void dfs(int u, int M, const vector<Node>& nodes, vector<vector<int>>& dp) {
    // 先将当前节点的学分放入背包
    for (int i = 1; i <= M; ++i) {
        dp[u][i] = nodes[u].score;
    }
    // 此方法中, 我们认为 dp[u][j] 表示在以 u 为根的子树中，必须选择 u，总共选择不超过 j 门课所能获得的最大学分
    // 因此在这里我们初始化了 dp[u][1~M] 为 nodes[u].score

    for (int s : nodes[u].sons) {
        dfs(s, M, nodes, dp);         // 先遍历子节点树, 确保 dp[s] ready
        for (int j = M; j > 1; --j) { // 这里必须倒序, 因为是0-1背包
            for (int k = 1; j - k >= 1; ++k) {
                dp[u][j] = max(dp[u][j], dp[u][j - k] + dp[s][k]);
            }
        }
    }
}

// M 表示选课程数量, k[i] 表示第 i 门课的直接先修课的编号, s[i] 表示第 i 门课的学分
// 编号从 1 开始, 对应课程数组中的第 i-1 个元素  0 表示没有先修课
int selectCourses_grouped_knapsack(int M, const vector<int>& k, const vector<int>& s) {
    // 因为给的数据是一片森林(多个树), 不好操作, 先把所有无先修课的课程都设置一个公共的先修课--节点0
    int N = k.size() + 1;
    M++; // 因为额外加了一门虚拟节点0, 所以这里 M +1
    vector<Node> nodes;
    nodes.reserve(N);
    nodes.emplace_back(0, -1); // 虚拟根节点
    for (int i = 0; i < int(k.size()); ++i) {
        nodes.emplace_back(s[i], k[i]); // 这里 k[i] 直接是 nodes 中的序号
    }
    for (int i = 1; i < (int)nodes.size(); ++i) {
        nodes[nodes[i].parent].sons.emplace_back(i);
    }

    // dp数组,
    vector<vector<int>> dp(N, vector<int>(M + 1, 0));

    dfs(0, M, nodes, dp);

    return dp[0][M];
}

// 解法2: dfs 小优化

// u - 当前节点
int dfs2(int u, int M, const vector<Node>& nodes, vector<vector<int>>& dp) {
    int nu = 1;

    // 先将当前节点放入背包
    dp[u][1] = nodes[u].score;

    for (int s : nodes[u].sons) {
        int ns = dfs2(s, M, nodes, dp); // 先遍历子节点树, 确保 dp[s] ready

        // 优化边界, 让 1<=j-k<=nu, 1<=k<=ns, j<= nu+ns 且 j <= M
        for (int j = min(nu + ns, M); j > 1; --j) { // 这里必须倒序, 因为是0-1背包
            int lim = min(ns, j - 1);
            for (int k = max(1, j - nu); k <= lim; ++k) {
                dp[u][j] = max(dp[u][j], dp[u][j - k] + dp[s][k]);
            }
        }
        nu += ns;
    }
    return nu; // 返回当前树的节点数
}

// M 表示选课程数量, k[i] 表示第 i 门课的直接先修课的编号, s[i] 表示第 i 门课的学分
// 编号从 1 开始, 对应课程数组中的第 i-1 个元素  0 表示没有先修课
int selectCourses_grouped_knapsack2(int M, const vector<int>& k, const vector<int>& s) {
    // 因为给的数据是一片森林(多个树), 不好操作, 先把所有无先修课的课程都设置一个公共的先修课--节点0
    int N = k.size() + 1;
    M++; // 因为额外加了一门虚拟节点0, 所以这里 M +1
    vector<Node> nodes;
    nodes.reserve(N);
    nodes.emplace_back(0, -1); // 虚拟根节点
    for (int i = 0; i < int(k.size()); ++i) {
        nodes.emplace_back(s[i], k[i]); // 这里 k[i] 直接是 nodes 中的序号
    }
    for (int i = 1; i < (int)nodes.size(); ++i) {
        nodes[nodes[i].parent].sons.emplace_back(i);
    }

    // dp数组,
    vector<vector<int>> dp(N, vector<int>(M + 1, 0));

    dfs2(0, M, nodes, dp);

    return dp[0][M];
}

// 解法3: dfs序解法

// 获取dfs后序遍历编号以及节点树大小
void dfs3(int u, vector<Node>& nodes, vector<int>& awa) {
    for (int s : nodes[u].sons) {
        dfs3(s, nodes, awa);
        nodes[u].siz += nodes[s].siz;
    }
    awa.emplace_back(u);
}

int selectCourses_dfs(int M, const vector<int>& k, const vector<int>& s) {
    int N = k.size() + 1;
    M++;
    vector<Node> nodes;
    nodes.reserve(N);
    nodes.emplace_back(0, -1); // 虚拟根节点
    for (int i = 0; i < int(k.size()); ++i) {
        nodes.emplace_back(s[i], k[i]); // 这里 k[i] 直接是 nodes 中的序号
    }
    for (int i = 1; i < (int)nodes.size(); ++i) {
        nodes[nodes[i].parent].sons.emplace_back(i);
    }

    vector<int> awa; // 后序遍历序号
    awa.reserve(N);
    dfs3(0, nodes, awa);

    // dp数组, f[i][j] 表示前i个节点中选j个的最大值
    vector<vector<int>> f(N + 1, vector<int>(M + 1, 0));
    for (int i = 1; i <= N; ++i) {
        for (int j = M; j > 0; --j) {
            f[i][j] = max(f[i - 1][j - 1] + nodes[awa[i - 1]].score, f[i - nodes[awa[i - 1]].siz][j]);
        }
    }
    // 注: awa索引从0开始,所以这里需i-1

    return f[N][M];
}

// 解法4: 多叉转二叉树解法

void conv(int u, vector<Node>& nodes) {
    int pre = -1;
    for (int s : nodes[u].sons) {
        if (pre == -1) {
            nodes[u].left = s;
        } else {
            nodes[pre].right = s;
        }
        pre = s;
        conv(s, nodes);
    }
}

void cntsiz(int u, vector<Node>& nodes) {
    if (nodes[u].left != -1) {
        cntsiz(nodes[u].left, nodes);
        nodes[u].siz += nodes[nodes[u].left].siz;
    }
    if (nodes[u].right != -1) {
        cntsiz(nodes[u].right, nodes);
        nodes[u].siz += nodes[nodes[u].right].siz;
    }
}

// 二叉树遍历
void dfs4(int u, int M, const vector<Node>& nodes, vector<vector<int>>& dp) {
    int l = nodes[u].left, r = nodes[u].right;
    int lsiz = 0, rsiz = 0;
    if (l != -1) {
        lsiz = nodes[l].siz;
        dfs4(l, M, nodes, dp);
    }
    if (r != -1) {
        rsiz = nodes[r].siz;
        dfs4(r, M, nodes, dp);
    }
    int ilim = min(M, nodes[u].siz);
    for (int i = 1; i <= ilim; ++i) {
        if (l != -1 && r != -1) { // 两个子树都存在
            dp[u][i] = dp[r][i];  //不选当前节点的情况
                                  // 注：不需要 dp[r][min(i, rsiz)]，因为如果 i > rsiz，说明右子树已经分配满了，
                                  // 后续的 max 会处理，这句话就相当于没用了
            // 选当前节点的情况，i-1个节点从左右子树中挑
            // 这两个边界的核心是保证 dp[l][j] 的 j 在 [0, lsiz] 范围内
            // 和 dp[r][i-1-j] 的 i-1-j 在 [0, rsiz] 范围内
            // 超出范围的情况是无意义的
            int lj = max(0, i - 1 - rsiz);
            int rj = min(i - 1, lsiz);
            for (int j = lj; j <= rj; ++j) {
                // 左边 j个，右边 i-1-j个
                dp[u][i] = max(dp[u][i], dp[l][j] + dp[r][i - 1 - j] + nodes[u].score);
            }
        } else if (l != -1) {                         // 只有左子树
            dp[u][i] = dp[l][i - 1] + nodes[u].score; // 必须选当前节点才能选左子树
        } else if (r != -1) {                         // 只有右子树
            dp[u][i] = max(dp[r][i - 1] + nodes[u].score, dp[r][i]);
        } else {
            dp[u][i] = nodes[u].score; // 只有当前节点
        }
    }
}

int selectCourses_binary_tree(int M, const vector<int>& k, const vector<int>& s) {
    int N = k.size() + 1;
    M++;
    vector<Node> nodes;
    nodes.reserve(N);
    nodes.emplace_back(0, -1); // 虚拟根节点
    for (int i = 0; i < int(k.size()); ++i) {
        nodes.emplace_back(s[i], k[i]); // 这里 k[i] 直接是 nodes 中的序号
    }
    for (int i = 1; i < (int)nodes.size(); ++i) {
        nodes[nodes[i].parent].sons.emplace_back(i);
    }

    conv(0, nodes);
    cntsiz(0, nodes);

    vector<vector<int>> dp(N, vector<int>(M + 1, 0));
    dfs4(0, M, nodes, dp);

    return dp[0][M];
}

// 泛化物品求并

// dep 表示从虚拟根节点到当前节点的路径上真实课程的数量
void dfs5(int u, int M, int dep, const vector<Node>& nodes, vector<vector<int>>& dp) {
    for (int s : nodes[u].sons) {
        // 遍历子节点 s
        for (int j = dep + 1; j <= M; ++j) {
            dp[s][j] = dp[u][j - 1] + nodes[s].score;
        }

        // 递归遍历 s 子树的所有节点
        dfs5(s, M, dep + 1, nodes, dp);

        // 此时 dp[s] 表示选s及u的方案, dp[u] 表示选u不选s的方案,两者仅可取其一
        for (int j = dep + 1; j <= M; ++j) {
            dp[u][j] = max(dp[u][j], dp[s][j]);
        }
    }
}

int selectCourses_generalized_items(int M, const vector<int>& k, const vector<int>& s) {
    int N = k.size() + 1;
    vector<Node> nodes;
    nodes.reserve(N);
    nodes.emplace_back(0, -1); // 虚拟根节点
    for (int i = 0; i < int(k.size()); ++i) {
        nodes.emplace_back(s[i], k[i]); // 这里 k[i] 直接是 nodes 中的序号
    }
    for (int i = 1; i < (int)nodes.size(); ++i) {
        nodes[nodes[i].parent].sons.emplace_back(i);
    }

    // dp[u][j] 表示遍历到 u 节点时，选择 j 真实门课且必选 u 的最大学分
    vector<vector<int>> dp(N, vector<int>(M + 1, 0));
    dfs5(0, M, 0, nodes, dp);

    return dp[0][M]; // 注意这里的 M 没有 +1, 因为 dp[u][j] 里的 j 表示的是选的真实课程数
}

#include "test_utils.hpp"

int main() {
    cout << "Select Courses Problem" << endl;
    cout << "TestCase: (M, k, s, Expected)" << endl;

    using TestCase = tuple<int, vector<int>, vector<int>, int>;
    vector<TestCase> test_cases{
        {4, {2, 0, 0, 2, 7, 7, 2},    {2, 1, 4, 1, 1, 6, 2},    13},
        {4, {0, 1, 2, 3, 4, 0, 0, 0}, {1, 2, 3, 4, 5, 6, 7, 8}, 22},
    };

    vector<pair<string, decltype(&selectCourses_grouped_knapsack)>> methods = {
        {"selectCourses_grouped_knapsack",  selectCourses_grouped_knapsack },
        {"selectCourses_grouped_knapsack2", selectCourses_grouped_knapsack2},
        {"selectCourses_dfs",               selectCourses_dfs              },
        {"selectCourses_binary_tree",       selectCourses_binary_tree      },
        {"selectCourses_generalized_items", selectCourses_generalized_items},
    };

    runTests(methods, test_cases);

    return 0;
}
