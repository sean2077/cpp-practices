#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
int n, m, v[100010];
vector<int> G[100010], f[100010];

void dfs(int u, int dep) {
    for (auto s : G[u]) {
        for (int j = dep + 1; j <= m; j++) {
            f[s][j] = f[u][j - 1] + v[s];
        }
        dfs(s, dep + 1);
        for (int j = dep + 1; j <= m; j++) {
            f[u][j] = max(f[u][j], f[s][j]);
        }
    }
}

int main() {
    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        int u;
        cin >> u >> v[i];
        G[u].emplace_back(i);
    }
    for (int i = 0; i <= n; i++) {
        f[i].resize(m + 1);
    }
    dfs(0, 0);
    cout << f[0][m];
    return 0;
}
