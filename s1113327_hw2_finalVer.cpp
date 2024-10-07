#include <iostream>
#include <vector>
#include <cstring> // For memset
using namespace std;

struct Edge {
    int to, nxt;
};

const int MAXN = 10;
const int MAXE = 500;

long long int dp[1 << 9][10]; // Keep this as a global variable due to size constraints

void addEdge(vector<Edge>& edges, vector<int>& head, int& cntEdge, int u, int v) {
    edges[++cntEdge] = { v, head[u] };
    head[u] = cntEdge;
}

bool isConnected(const vector<Edge>& edges, const vector<int>& head, int n) {
    vector<bool> visited(n + 1, false);
    int components = 0;

    auto dfs = [&](int node, auto&& dfs_ref) -> void {
        visited[node] = true;
        for (int i = head[node]; i; i = edges[i].nxt) {
            int v = edges[i].to;
            if (!visited[v]) {
                dfs_ref(v, dfs_ref);
            }
        }
        };

    for (int i = 1; i <= n; ++i) {
        if (!visited[i]) {
            components++;
            if (components > 1) {
                return false; // More than one component, not connected
            }
            dfs(i, dfs);
        }
    }
    return true;
}

int main() {
    int n = 0;
    while (cin >> n) {
        if (n < 1) {
            continue;
        }

        vector<Edge> edges(MAXE);
        vector<int> head(MAXN, 0);
        int cntEdge = 0;
        long long int answer = 0;
        int m = 0;
        int u = 0, v = 0;
        memset(dp, 0, sizeof(dp)); // Reset dp array
        while (cin >> u) {
            if (u < 0)
                break;
            cin >> v;
            m++;
            addEdge(edges, head, cntEdge, u, v);
            addEdge(edges, head, cntEdge, v, u);
        }

        if (!isConnected(edges, head, n)) {
            cout << "n" << endl;
            continue;
        }

        for (int i = 1; i <= n; i++)
            dp[1 << (i - 1)][i] = 1;

        for (int s = 1; s < (1 << n); s++) {
            for (int i = 1; i <= n; i++) {
                if (!dp[s][i])
                    continue;
                for (int j = head[i]; j; j = edges[j].nxt) {
                    int u = i, v = edges[j].to;
                    if ((s & -s) > (1 << (v - 1)))
                        continue;
                    if (s & (1 << (v - 1))) {
                        if ((s & -s) == (1 << (v - 1)))
                            answer += dp[s][u];
                    }
                    else
                        dp[s | (1 << (v - 1))][v] += dp[s][u];
                }
            }
        }

        if ((answer - m) / 2 >= 2) {
            cout << "y" << endl;
            if ((answer - m) / 2 >= 3)
                cout << "There are at least three cycles." << endl;
        }
        else
            cout << "n" << endl;
    }
    return 0;
}