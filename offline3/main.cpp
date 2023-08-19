#include<iostream>
#include<fstream>
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

vector<int> semigreedy_maxcut(int n_v, vector<Edge> edges, Graph graph){
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

    return solution;
}

vector<int> localSearch_maxcut(
    vector<int> initial_solution, 
    int n_v, vector<Edge> edges, Graph graph
){
    bool change = true;

    while(change){
        change = false;
        for(int i = 1; i <= n_v; i++){
            int sigmaX = 0;
            int sigmaY = 0;

            for(auto edge: graph[i]){
                int w = edge.first;
                int otherend = edge.second;

                if(initial_solution[otherend] == 1) sigmaX += w;
                else if(initial_solution[otherend] == 2) sigmaY += w;
            }

            if(initial_solution[i] == 1 && sigmaX > sigmaY){
                initial_solution[i] = 2;
                change = true;
            }else if(initial_solution[i] == 2 && sigmaY > sigmaX){
                initial_solution[i] = 1;
                change = true;
            }
        }
    }

    return initial_solution;
}

int main(int argc, char* argv[]){
    string problemname = "N/A";
    if(argc == 2){
        problemname = argv[1];
    }
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

    int sum_rand = 0, sum_semigreedy = 0, sum_local = 0;
    int best_semigreedy = INT32_MIN;
    int no_of_iteration = 100;

    cout << "running greedy " << endl;
    int greedy = greedy_maxcut(n_v, edges);
    cout << "Greedy\t\t: " << greedy << endl;

    for(int i = 0; i < no_of_iteration; i++){
        cout << "iteration " << i << endl << endl;

        int random = randomized_maxcut(n_v, edges);
        sum_rand += random;
        cout << "random\t\t: " << random << endl;

        vector<int> semigreedy = semigreedy_maxcut(n_v, edges, graph);
        sum_semigreedy += find_cut_weight(semigreedy, edges);
        cout << "semigreedy\t: " 
            << find_cut_weight(semigreedy, edges) << endl;

        
        vector<int> localSearch = localSearch_maxcut(
            semigreedy, n_v, edges, graph
        );
        int local_search_score = find_cut_weight(localSearch, edges);
        best_semigreedy = max(local_search_score, best_semigreedy);
        sum_local += local_search_score;
        cout<< "localSearch\t: " 
            << local_search_score << endl;

        cout << endl;
    }

    cout << "GRASP\t\t: " << best_semigreedy << endl;
    
    cout << "\nSUMMARY" << endl;
    cout << "================================" << endl;
    cout << "Average random\t\t: " << (double)sum_rand / no_of_iteration << endl;
    cout << "Greedy\t\t\t: " << greedy << endl;
    cout << "Average semigreedy\t: " << (double)sum_semigreedy / no_of_iteration << endl;
    cout << "Average localsearch\t: " << (double)sum_local / no_of_iteration << endl;
    cout << "GRASP\t\t\t: " << best_semigreedy << endl;

    ofstream report("results.csv", ios_base::app);
    report << problemname << ","
           << n_v << ","
           << n_e << ","
           << (double)sum_rand / no_of_iteration << ","
           << greedy << ","
           << (double)sum_semigreedy / no_of_iteration << ","
           << (double)sum_local / no_of_iteration << ","
           << no_of_iteration << ","
           << best_semigreedy << ","
           << no_of_iteration << ","
           << endl;
    report.close();
}