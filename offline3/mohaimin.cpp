#include<bits/stdc++.h>
#include<fstream>

using namespace std;

#define MAX_NODES (int)1e5
#define MAX_WEIGHT 100

int N, M;
vector<pair<int, int>> adj[MAX_NODES+1];
set<int> X, Y, Z;

int getWeight(int, int);
int getWeight(set<int>, set<int>);
int getSigma(int, set<int>);

void semiGreedyMaxCut(float);
void localSearchMaxCut();

int main() {

    freopen ("./benchmark/g1.rud", "r", stdin);
    ofstream ReportFile("report.csv");

    cin >> N >> M;

    for (int i = 1; i <= N; i++) Z.insert(i);

    // BUILD GRAPH
    for (int i = 0; i < M; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w}); 
        // adj[v].push_back({u, w}); 
    }

    ReportFile << "SemiGreedy,LocalSearch\n";

    semiGreedyMaxCut(0.2);

    // TODO: generate report
    ReportFile << getWeight(X, Y) << ",";

    // for (auto x: X) cout << x << " ";
    // cout << endl;
    // for (auto y: Y) cout << y << " ";
    // cout << endl;

    localSearchMaxCut();

    // TODO: generate report
    ReportFile << getWeight(X, Y) << "\n";

    // for (auto x: X) cout << x << " ";
    // cout << endl;
    // for (auto y: Y) cout << y << " ";
    // cout << endl;

    ReportFile.close();
    return 0;
}

int getWeight(int u, int v) {
    for (auto x: adj[u]) {
        if (x.first == v) return x.second;
    }

    for (auto x: adj[v]) {
        if (x.first == u) return x.second;
    }

    return -MAX_WEIGHT-3;
}

int getWeight(set<int> A, set<int> B) {
    int weight = 0;

    for (auto a: A) {
        for (auto b: B) {
            int w = getWeight(a, b);
            if (w > -MAX_WEIGHT) weight += w;
        }
    }

    return weight;
}

int getSigma(int x, set<int> Y) {
    int sigma = 0;

    for (auto y: Y) {
        int w = getWeight(x, y);
        if (w > -MAX_WEIGHT)
            sigma += w;
    }

    return sigma;
}

void semiGreedyMaxCut(float alpha) { 
    // srand(1935);
    if (alpha < 0) alpha = (rand() % 100)/100.0;

    int wMin = (int)INFINITY, wMax = -(int)INFINITY;
    for (int i = 1; i <= N; i++) {
        for (auto x: adj[i]) {
            wMin = min(wMin, x.second);
            wMax = max(wMax, x.second);
        }
    }

    // generate rcle
    int threshold = wMin + alpha*(wMax - wMin);
    vector<pair<int, int>> rclEdgeList;
    for (int i = 1; i <= N; i++) {
        for (int j = i+1; j <= N; j++) {
            if (getWeight(i, j) >= -MAX_WEIGHT && getWeight(i, j) >= threshold) {
                rclEdgeList.push_back({i, j});
            }
        }
    }

    // select random edge from rcle
    int edgeIndex = rand() % rclEdgeList.size();
    X.insert(rclEdgeList[edgeIndex].first);
    Y.insert(rclEdgeList[edgeIndex].second);
    Z.erase(rclEdgeList[edgeIndex].first);
    Z.erase(rclEdgeList[edgeIndex].second);

    while (Z.size()) {
        cout << "semi greedy: " << ((N-Z.size())/(float)N)*100 << "% done" << endl;
        wMin = (int)INFINITY, wMax = -(int)INFINITY;
        for (auto z: Z) {
            int sigmaX = getSigma(z, Y);
            int sigmaY = getSigma(z, X);
            int mi = min(sigmaX, sigmaY);
            int ma = max(sigmaX, sigmaY);
            wMin = min(wMin, mi);
            wMax = max(wMax, ma);
        }

        // generate rclv
        threshold = wMin + alpha*(wMax - wMin);
        vector<int> rclVertexList;
        for (auto z: Z) {
            int sigmaX = getSigma(z, Y);
            int sigmaY = getSigma(z, X);

            if (max(sigmaX, sigmaY) >= threshold) {
                rclVertexList.push_back(z);
            }
        }

        // select random vertex from rclv
        int vertexIndex = rand() % rclVertexList.size();
        int selectedVertex = rclVertexList[vertexIndex];
        if (getSigma(selectedVertex, X) > getSigma(selectedVertex, Y)) Y.insert(selectedVertex);
        else X.insert(selectedVertex);
        Z.erase(selectedVertex);
    }
}

void localSearchMaxCut() {
    bool change = true;

    while (change) {
        change = false;

        cout << "local searching...\n";

        for (int i = 1; i <= N && !change; i++) {
            int sigmaX = getSigma(i, Y);
            int sigmaY = getSigma(i, X);
            
            if (X.count(i) && sigmaY > sigmaX) {
                X.erase(i);
                Y.insert(i);
                change = true;
            } else if (Y.count(i) && sigmaX > sigmaY) {
                Y.erase(i);
                X.insert(i);
                change = true;
            }
        }
    }
}