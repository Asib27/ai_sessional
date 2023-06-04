#include <iostream>
#include <vector>

using namespace std;

class Board
{
private:
    vector<vector<int>> board;

    vector<int> flatten(){
        vector<int> v;

        for(auto &i: board){
            for(auto j: i){
                if( j != 0)
                    v.push_back(j);
            }
        }

        return v;
    }

    int getGoalNumber(int row, int col, int size){
        if(row == size-1 && col == size-1) return 0;
        return row*size + col + 1;
    }

    int findInversionCount(){
        int inversion = 0;
        vector<int> flat = flatten();

        for(int i = 0; i < flat.size(); i++){
            for(int j = i+1; j < flat.size(); j++){
                if(flat[i] > flat[j]) inversion++;
            }
        }

        return inversion;
    }

    int getBlankSquareRow(){
        int size = getSize();

        for(int i = 0; i < size; i++){
            for(auto j: board[i]){
                if(j == 0) return i;
            }
        }

        return -1;
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

    Board(vector<vector<int>> board){
        this->board = board;
    }

    int getNumber(int row, int col){
        return board[row][col];
    }

    int getSize(){
        return board.size();
    }

    int getHammingDistance(){
        return 0;
    }

    int getManhattenDistance(){
        return 0;
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
        return vector<Board> ();
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
};

void isGoalTest(){
    vector<vector<int>> goal = {vector<int>{1,2,3}, vector<int>{4,5,6}, vector<int>{7,8,9}};
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

int main(){
    isGoalTest();
    isSolvableTest();
    is4SolvableTest();
}