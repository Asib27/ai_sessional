#include<iostream>
#include<vector>
#include<utility>
#include<algorithm>
#include<cstdlib>
#include<cmath>
#include<set>

using namespace std;

typedef pair<int, pair<int,int>> Edge;
typedef vector<vector<pair<int,int>>> Graph;

int find_cut_weight(vector<int> &solution, vector<Edge> &edges){
    int cut = 0;
    for(auto i: edges){
        int w = i.first;
        int a = i.second.first;
        int b = i.second.second;

        if(solution[a] && solution[b] && solution[a] != solution[b]) cut += w;
    }

    return cut;
}

int greedy_maxcut(int n_v, vector<Edge> edges){
    sort(edges.rbegin(), edges.rend());

    vector<int> solution(n_v+1, 0);
    int used_count = 0;
    int cut_weight = 0;

    for(auto i: edges){
        int w = i.first;
        int a = i.second.first;
        int b = i.second.second;

        // a -> 1, b -> 2
        solution[a] = 1;
        solution[b] = 2;
        int cut1 = find_cut_weight(solution, edges);
        
        // a -> 2, b -> 1
        solution[a] = 2;
        solution[b] = 1;
        int cut2 = find_cut_weight(solution, edges);
        
        // take the maximum
        if(cut1 > cut2){
            solution[a] = 1;
            solution[b] = 2;
        }
    }

    return find_cut_weight(solution, edges);
}

int randomized_maxcut(int n_v, vector<Edge> edges){
    vector<int> solution(n_v+1, 0);

    for(auto &i: solution){
        i = rand() % 2+1;
    }

    return find_cut_weight(solution, edges);
}

int semigreedy_maxcut(int n_v, vector<Edge> edges, Graph graph){
    vector<int> solution(n_v+1);
    set<int> rem;

    double alpha = .5;

    // finding threshold
    int mn = INT32_MAX;
    int mx = INT32_MIN;

    for(auto i: edges){
        mn = min(i.first, mn);
        mx = max(i.first, mx);
    }

    int thres = mn + (mx-mn) * alpha;

    // finding random first edge
    vector<Edge> rclEdge;

    for(auto &i:edges){
        if(i.first >= thres){
            rclEdge.push_back(i);
        }
    }
    int randEdge = rand() % rclEdge.size();

    // inserting the edge
    Edge edge = edges[randEdge];
    solution[edge.second.first] = 1;
    solution[edge.second.second] = 2;

    // initialize remaining set
    for(int i = 0; i <= n_v; i++){
        rem.insert(i);
    }
    rem.erase(edge.second.first);
    rem.erase(edge.second.second);

    // inserting other vertex
    while(rem.size() != 1){
        vector<int> sigmaX(n_v+1, 0), sigmaY(n_v+1, 0);

        for(auto node: rem){
            for(auto edge: graph[node]){
                int otherend = edge.second;
                int w = edge.first;

                if(solution[otherend] == 1) sigmaX[node] += w;
                else if(solution[otherend] == 2) sigmaY[node] += w;
            }
        }

        // finding new threshold
        int mn = INT32_MAX;
        int mx = INT32_MIN;
        for(auto node: rem){
            mn = min({mn, sigmaX[node], sigmaY[node]});
            mx = max({mx, sigmaX[node], sigmaY[node]});
        }
        thres = mn + (mx - mn) * alpha;

        // selecting random vertex
        vector<int> rclVertex;

        for(auto node: rem){
            if( max(sigmaX[node], sigmaY[node]) >= thres ){
                rclVertex.push_back(node);
            }
        }
        int selectedVertexIdx = rand() % rclVertex.size();
        int selectedVertex = rclVertex[selectedVertexIdx];

        if(sigmaX[selectedVertex] > sigmaY[selectedVertex]){
            solution[selectedVertex] = 2;
        }else{
            solution[selectedVertex] = 1;
        }
        rem.erase(selectedVertex);
    }

    return find_cut_weight(solution, edges);
}

int main(){
    srand(1927);

    int n_v, n_e;
    cin >> n_v >> n_e;

    Graph graph(n_v+1);
    vector<Edge> edges;
    for(int i = 0; i < n_e; i++){
        int a,b,w;
        cin >> a >> b >> w;
        graph[a].push_back({w,b});
        graph[b].push_back({w,a});

        edges.push_back({w, {a,b}});
    }

    int random = randomized_maxcut(n_v, edges);
    cout << random << endl;
    int greedy = greedy_maxcut(n_v, edges);
    cout << greedy << endl;
    int semigreedy = semigreedy_maxcut(n_v, edges, graph);
    cout << semigreedy << endl;
}