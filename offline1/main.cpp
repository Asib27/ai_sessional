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

public:
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

    }

    int getManhattenDistance(){

    }

    bool isGoal(){
        
    }

    bool operator ==(Board b){
        return b.board == this->board;
    }

    vector<Board> getNeighbours(){

    }

    bool isSolvable(){
        
    }

    friend ostream& operator <<(ostream &os, Board &board){
        for(auto &i: board.board){
            for(auto j: i){
                os << j << " ";
            }
            os << endl;
        }

        return os;
    }
};



int main(){
    cout << "hello world" << endl;
}