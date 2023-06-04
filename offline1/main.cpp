#include <iostream>
#include <vector>

using namespace std;

class Board
{
private:
    vector<vector<int>> board;

    vector<int> flat(){
        vector<int> v;

        for(auto &i: board){
            for(auto j: i){
                v.push_back(j);
            }
        }

        return v;
    }

    int getGoalNumber(int row, int col, int size){
        if(row == size-1 && col == size-1) return 0;
        return row*size + col + 1;
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
        return true;
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
}

int main(){
    isGoalTest();
}