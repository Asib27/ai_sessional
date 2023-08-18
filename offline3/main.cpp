#include<iostream>
#include<vector>
#include<utility>
#include<algorithm>
#include<cstdlib>

using namespace std;

typedef pair<int, pair<int,int>> Edge;

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

int main(){
    srand(1927);

    int n_v, n_e;
    cin >> n_v >> n_e;

    vector<vector<pair<int,int>>> graph(n_v+1);
    vector<Edge> edges;
    for(int i = 0; i < n_e; i++){
        int a,b,w;
        cin >> a >> b >> w;
        graph[a].push_back({w,b});
        graph[b].push_back({w,a});

        edges.push_back({w, {a,b}});
    }

    int random = randomized_maxcut(n_v, edges);
    int greedy = greedy_maxcut(n_v, edges);
    cout << random  << " , " << greedy << endl;
}