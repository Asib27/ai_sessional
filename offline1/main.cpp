#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class Board
{
private:
    vector<vector<int>> board;
    int manhatten_distance;
    int hamming_distance;
    int g_score;
    size_t hash_value;

    vector<int> flatten() const{
        vector<int> v;

        for(auto &i: board){
            for(auto j: i){
                if( j != 0)
                    v.push_back(j);
            }
        }

        return v;
    }

    int getGoalNumber(int row, int col, int size)const {
        if(row == size-1 && col == size-1) return 0;
        return row*size + col + 1;
    }

    int findInversionCount() const{
        int inversion = 0;
        vector<int> flat = flatten();

        for(int i = 0; i < flat.size(); i++){
            for(int j = i+1; j < flat.size(); j++){
                if(flat[i] > flat[j]) inversion++;
            }
        }

        return inversion;
    }

    int getBlankSquareRow() const{
        int size = getSize();

        for(int i = 0; i < size; i++){
            for(auto j: board[i]){
                if(j == 0) return i;
            }
        }

        return -1;
    }

    int getBlankSquareColumn(int row) const{
        int size = getSize();

        for(int i = 0; i < size; i++){
            if(getNumber(row, i) == 0) return i;
        }

        return -1;
    }

    int getManhattenDistanceForCell(int row, int col) const{
        int size = getSize();
        int val = getNumber(row, col);

        if(val == 0) return 0;
        
        // val is 1 indexed
        int target_row = (val-1) / size;
        int target_col = (val-1) % size;

        return abs(target_row - row) + abs(target_col - col);
    }

    inline size_t _getHash() const{
        int size = getSize();
        size_t hash = 0;

        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                hash += getNumber(i, j) * (i*size+j);
            }
        }

        return hash;
    }

public:
    static Board getGoalBoard(int gridSize){
        vector<vector<int>> grid(gridSize, vector<int>(gridSize));

        for(int i = 0; i < gridSize; i++){
            for(int j = 0; j < gridSize; j++){
                grid[i][j] = i*gridSize + j + 1;
            }
        }

        grid[gridSize-1][gridSize-1] = 0;
        return Board(grid);
    }

    Board(vector<vector<int>> board, int gScore=0)
        : Board()
    {
        this->board = board;
        this->g_score = gScore;
        this->hash_value = _getHash();
    }

    Board(){
        manhatten_distance = -1;
        hamming_distance = -1;
        g_score = 0;
        hash_value = -1;
    }

    inline void setgscore(int score){
        g_score = score;
    }

    inline int getgscore() const{
        return g_score;
    }

    inline int getNumber(int row, int col) const{
        return board[row][col];
    }

    inline int getSize() const{
        return board.size();
    }

    inline size_t getHash() const{
        return hash_value;
    }

    int getHammingDistance(){
        if(hamming_distance != -1) return hamming_distance;

        int dist = 0;
        int size = getSize();

        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                if(board[i][j] != 0)
                    dist += (board[i][j] != getGoalNumber(i, j, size)); 
            }
        }

        hamming_distance = dist;
        return dist;
    }

    int getManhattenDistance(){
        if(manhatten_distance != -1) return manhatten_distance;

        int size = getSize();
        int dist = 0;

        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                dist += getManhattenDistanceForCell(i, j);
            }
        }

        manhatten_distance = dist;
        return dist;
    }

    bool isGoal(){
        int size = getSize();

        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                if(board[i][j] != getGoalNumber(i, j, size))
                    return false;
            }
        }

        return true;
    }

    bool operator ==(Board b){
        return b.board == this->board;
    }

    vector<Board> getNeighbours(){
        int size = getSize();
        int row = getBlankSquareRow();
        int col = getBlankSquareColumn(row);

        vector<Board> neighbours;

        for(auto i : vector<pair<int,int>>({{1,0}, {-1,0}, {0,1}, {0,-1}})){
            int nei_row = row + i.first;
            int nei_col = col + i.second;

            if(nei_row < 0 || nei_row >= size) continue;
            if(nei_col < 0 || nei_col >= size) continue;

            vector<vector<int>> nei_board(board);
            nei_board[row][col] = board[nei_row][nei_col];
            nei_board[nei_row][nei_col] = 0;

            neighbours.emplace_back(nei_board, this->getgscore()+1);
        }

        return neighbours;
    }

    bool isSolvable(){
        int inversions = findInversionCount();
        int size = getSize();

        if(size%2 == 1){
            return inversions%2 == 0;
        }else{
            int blankSquareRow = getBlankSquareRow();
            return (inversions+blankSquareRow) % 2;
        }
    }

    friend ostream& operator <<(ostream &os, Board &board){
        for(auto &i: board.board){
            for(auto j: i){
                if(j != 0)
                    os << j << " ";
                else os << "* ";
            }
            os << endl;
        }

        return os;
    }

    bool operator==(const Board &board) const{
        if(this->getSize() != board.getSize()) return false;

        int n = this->getSize();
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                if(this->getNumber(i, j) != board.getNumber(i, j)) return false;
            }
        }

        return true;
    }
};

class ManhattenCompare {
    public:
       bool operator()(Board a, Board b){
            int afscore = a.getManhattenDistance() + a.getgscore();
            int bfscore = b.getManhattenDistance() + b.getgscore();
            if(afscore > bfscore){
                return true;
            }
            return false;
      }
};

class HammingCompare {
    public:
       bool operator()(Board &a, Board &b){
            int afscore = a.getHammingDistance() + a.getgscore();
            int bfscore = b.getHammingDistance() + b.getgscore();
            if(afscore > bfscore){
                return true;
            }
            return false;
      }
};

class BoardHash{
    public:
       size_t operator()(const Board &board)const{
           return board.getHash();
       }
};

void solveManhatten(Board initial, bool debug=false){
    int explored = 0;
    int expanded = 0;

    priority_queue<Board, vector<Board>, ManhattenCompare> openSet;

    openSet.push(initial);
    unordered_map<Board, Board, BoardHash> parent;
    unordered_set<Board, BoardHash> seen;
    seen.insert(initial);

    while(!openSet.empty()){
        Board current = openSet.top();
        expanded++;

        if(current.isGoal()){
            vector<Board> path;
            int dist = current.getgscore();

            while(parent.count(current)){
                path.push_back(current);
                current = parent[current];
            }
            path.push_back(current);

            for(int i = path.size()-1; i >= 0 ; i--){
                cout << path[i] << endl;
            }

            cout << dist << endl;

            cout << "Manhatten Heuristics" << endl;
            cout << "Expanded : " << expanded << endl;
            cout << "Explored : " << explored << endl;
            break;
        }

        openSet.pop();

        if(debug){
            cout << "=================================================" << endl;
            cout << current.getHammingDistance() + current.getgscore() << endl;
            cout << current << endl;
        }

        for(auto nei : current.getNeighbours()){
            if(!seen.count(nei)){
                parent[nei] = current;
                openSet.push(nei);
                seen.insert(nei);
                explored++;

                // cout << parent[nei] << endl;

                if(debug)
                    cout << nei << endl;
            }

        }

        if(debug){
            cout << "======================================================" << endl;
            int a;
            cin >> a;
        }
    }
}

void solveHamming(Board initial, bool debug=false){
    int explored = 0;
    int expanded = 0;

    priority_queue<Board, vector<Board>, HammingCompare> openSet;

    openSet.push(initial);
    unordered_map<Board, Board, BoardHash> parent;
    unordered_set<Board, BoardHash> seen;
    seen.insert(initial);

    while(!openSet.empty()){
        Board current = openSet.top();
        expanded++;
        if(current.isGoal()){
            vector<Board> path;
            int dist = current.getgscore();

            while(parent.count(current)){
                path.push_back(current);
                current = parent[current];
            }
            path.push_back(current);

            for(int i = path.size()-1; i >= 0 ; i--){
                cout << path[i] << endl;
            }

            cout << dist << endl;


            cout << "Hamming Heuristics" << endl;
            cout << "Expanded : " << expanded << endl;
            cout << "Explored : " << explored << endl;
            break;
        }

        openSet.pop();

        if(debug){
            cout << "=================================================" << endl;
            cout << current.getHammingDistance() + current.getgscore() << endl;
            cout << current << endl;
        }

        for(auto nei : current.getNeighbours()){
            if(!seen.count(nei)){
                parent[nei] = current;
                openSet.push(nei);
                seen.insert(nei);
                explored++;

                // cout << parent[nei] << endl;

                if(debug)
                    cout << nei << endl;
            }

        }

        if(debug){
            cout << "======================================================" << endl;
            int a;
            cin >> a;
        }
    }
}

void manhattenDistanceTest(){
    vector<vector<int>> goal = {vector<int>{1,2,3}, vector<int>{4,5,6}, vector<int>{7,0,8}};
    Board board(goal);
    if(board.getManhattenDistance() != 1) cout << board << board.getManhattenDistance() << endl;

    goal = {vector<int>{1,2,3}, vector<int>{4,5,6}, vector<int>{7,8,0}};
    board = Board(goal);
    if(board.getManhattenDistance() != 0) cout << board << board.getManhattenDistance() << endl;

    goal = {vector<int>{8,1,3}, vector<int>{4,0,2}, vector<int>{7,6,5}};
    board = Board(goal);
    if(board.getManhattenDistance() != 10) cout << board << board.getManhattenDistance() << endl;

    goal = {vector<int>{7,2,4}, vector<int>{6,0,5}, vector<int>{8,3,1}};
    board = Board(goal);
    if(board.getManhattenDistance() != 16) cout << board << board.getManhattenDistance() << endl;

    cout << "Manhatten Distance Test Done" << endl;
}

void hammingDistanceTest(){
    vector<vector<int>> goal = {vector<int>{1,2,3}, vector<int>{4,5,6}, vector<int>{7,0,8}};
    Board board(goal);
    if(board.getHammingDistance() != 1) cout << board;

    goal = {vector<int>{1,2,3}, vector<int>{4,5,6}, vector<int>{7,8,0}};
    board = Board(goal);
    if(board.getHammingDistance() != 0) cout << board;

    goal = {vector<int>{8,1,3}, vector<int>{4,0,2}, vector<int>{7,6,5}};
    board = Board(goal);
    if(board.getHammingDistance() != 5) cout << board;

    goal = {vector<int>{7,2,4}, vector<int>{6,0,5}, vector<int>{8,3,1}};
    board = Board(goal);
    if(board.getHammingDistance() != 7) cout << board;

    cout << "Hamming Distance Test Done" << endl;
}

void isGoalTest(){
    vector<vector<int>> goal = {vector<int>{1,2,3}, vector<int>{4,5,6}, vector<int>{7,0,8}};
    Board board(goal);
    if(board.isGoal() != 0) cout << board;

    goal = {vector<int>{1,2,3}, vector<int>{4,5,6}, vector<int>{7,8,0}};
    board = Board(goal);
    if(board.isGoal() != 1) cout << board;

    goal = {vector<int>{4,2,3}, vector<int>{5,1,6}, vector<int>{7,8,0}};
    board = Board(goal);
    if(board.isGoal() != 0) cout << board;

    cout << "goal test done" << endl;
}

void isSolvableTest(){
    vector<vector<int>> goal = {vector<int>{1,2,3}, vector<int>{4,5,6}, vector<int>{7,8,9}};
    Board board(goal);
    if(board.isSolvable() != 1) cout << board;

    goal = {vector<int>{1,2,3}, vector<int>{4,5,6}, vector<int>{8,0,7}};
    board = Board(goal);
    if(board.isSolvable() != 0) cout << board;


    goal = {vector<int>{1,2,3}, vector<int>{0,4,6}, vector<int>{8,5,7}};
    board = Board(goal);
    if(board.isSolvable() != 0) cout << board;


    goal = {vector<int>{0,1,3}, vector<int>{4,2,5}, vector<int>{7,8,6}};
    board = Board(goal);
    if(board.isSolvable() != 1) cout << board;

    cout << "solvable test done" << endl;
}


void is4SolvableTest(){
    vector<vector<int>> goal = {vector<int>{1,2,3,4}, vector<int>{5,6,7,8}, vector<int>{9,10,11,12}, vector<int>{13,14,15,0}};
    Board board(goal);
    if(board.isSolvable() != 1) cout << board;

    goal = {vector<int>{1,2,3,4}, vector<int>{5,6,7,8}, vector<int>{9,10,0,11}, vector<int>{13,14,15,12}};
    board = Board(goal);
    if(board.isSolvable() != 1) cout << board;


    goal = {vector<int>{1,2,3,4}, vector<int>{5,6,0,8}, vector<int>{9,10,7,11}, vector<int>{13,14,15,12}};
    board = Board(goal);
    if(board.isSolvable() != 1) cout << board;


    goal = {vector<int>{1,2,3,4}, vector<int>{5,6,7,8}, vector<int>{9,10,11,12}, vector<int>{13,15,14,0}};
    board = Board(goal);
    if(board.isSolvable() != 0) cout << board;

    cout << "4 solvable test done" << endl;
}

void neighboursTest(){
    vector<vector<int>> goal = {vector<int>{1,2,3}, vector<int>{4,0,6}, vector<int>{7,5,8}};
    Board board(goal);
    auto nei = board.getNeighbours();
    for(auto i: nei){
        cout << i << endl;
    }
}

int main(){
    // isGoalTest();
    // isSolvableTest();
    // is4SolvableTest();
    // hammingDistanceTest();
    // manhattenDistanceTest();
    // neighboursTest();

    vector<vector<int>> goal = {
        vector<int>{4,1,3}, 
        vector<int>{0,2,6}, 
        vector<int>{7,5,8}};


    int boardSize;
    cin >> boardSize;
    vector<vector<int>> boardMat(boardSize, vector<int>(boardSize));

    for(auto &i: boardMat){
        for(auto &j: i){
            cin >> j;
        }
    }

    Board board(boardMat);   

    if(board.isSolvable()){
        solveManhatten(board, false);
        solveHamming(board, false);
    }
    else{
        cout << "Insolvable Puzzle" << endl;
    }
}