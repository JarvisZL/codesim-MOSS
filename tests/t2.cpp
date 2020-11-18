#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int N = 300;
const int INF = 0x3f3f3f3f;
int dist[N];
int s[N];
int f[N];

struct EDGE {
    int a, b, w;
}edge[N];

int find(int x) {
    return f[x] == x ? x : f[x] = find(f[x]);
}

int n, m;
int pointNum;
int edgeCnt = 0;
bool cmp(const struct EDGE& a, const struct EDGE& b)
{
    return a.w < b.w;
}

int kruscal()
{
    int ret = 0; int cnt = 0;
    sort(edge, edge + edgeCnt, cmp);
    for (int i = 1; i <= pointNum;i++) f[i] = i;
    for (int i = 0; i < edgeCnt; i++) {
        int a = edge[i].a; int b = edge[i].b; int w = edge[i].w;
        a = find(a); b = find(b);
        if (a != b) {
            f[a] = b;
            ret += w;
            cnt++;
        }
    }
    if (cnt< pointNum - 1) return INF;

    return ret;
}

int main()
{
    cin >> pointNum;
    while (pointNum != 0) {
        edgeCnt = 0;
        for (int loop = 1; loop < pointNum; loop++) {
            char c; int num;
            cin >> c; cin >> num;
            int idx = c - 'A' + 1;

            for (int i = 0; i < num; i++) {
                char to; int val;
                cin >> to >> val;
                int toIdx = to - 'A' + 1;
                edge[edgeCnt].a = idx;
                edge[edgeCnt].b = toIdx;
                edge[edgeCnt].w = val;
                edgeCnt++;
            }
        }

        cout << kruscal() << endl;

        cin >> pointNum;
    }
}
