#include<iostream>
#include<iomanip>
#include<vector>
#include<numeric>

using namespace std;

class Mancala
{
private:
    /* data */
    const int SIZE;

    vector<int> p1;
    vector<int> p2;
    int p1_store, p2_store;
    int cur_turn;
    bool is_game_over;

    vector<int>& getPlayerVector(char c){
        if(c == 's'){
            if(cur_turn == 1) return p1;
            else return p2;
        }else{
            if(cur_turn == 1) return p2;
            else return p1;
        }
    }

    int& getPlayerStore(char c){
        if(c == 's'){
            if(cur_turn == 1) return p1_store;
            else return p2_store;
        }else{
            if(cur_turn == 1) return p2_store;
            else return p1_store;
        }
    }

    void check_game_over(){
        int sum1 = accumulate(p1.begin(), p1.end(), 0);
        int sum2 = accumulate(p2.begin(), p2.end(), 0);

        if(!sum1 || !sum2){

            p1_store += sum1;
            p2_store += sum2;
            is_game_over = true;
        }
    }

public:
    Mancala(/* args */)
        : p1(6, 4), p2(6, 4), p1_store(0), p2_store(0),
        cur_turn(1), SIZE(6), is_game_over(false)
    {

    }

    int play_turn(int move){
        if(is_game_over) return 3;
        if(move >= SIZE) return 1;

        vector<int>& self = getPlayerVector('s');
        vector<int>& opp = getPlayerVector('o');

        if(self[move] == 0) return 2;

        int& self_store = getPlayerStore('s');
        int& opp_store = getPlayerStore('o');

        int value = self[move];
        self[move] = 0;
        int last = 0;

        while(value > 0){
            for(int i = move+1; value > 0 && i < SIZE; i++){
                self[i]++;
                last = i;
                value--;
            }

            if(value > 0){
                self_store++;
                last = 6;
                value--;
            }

            for(int i = 0; value > 0 && i < SIZE; i++){
                opp[i]++;
                value--;
                last = 7;
            }

            for(int i = 0; value > 0 && i < move+1; i++){
                self[i]++;
                value--;
                last = i;
            }
        }

        // get another turn
        if(last != 6){
            cur_turn = cur_turn == 1? 2 : 1;
        }

        // if last piece falls on empty
        if(last <= 5 && self[last] == 1){
            self_store += self[last] + opp[SIZE-last-1];
            self[last] = 0;
            opp[SIZE-last-1] = 0;
        }

        check_game_over();
        return 0;
    }
    
    int win(){
        if(is_game_over){
            return p1_store > p2_store? 1: 2;
        }
        return -1;
    }

    int current_turn(){
        return cur_turn;
    }

    friend ostream& operator<<(ostream &os, Mancala &m){
        os << "   ";
        for(int i = m.p2.size() -1; i >= 0; i--){
            os << setw(2) << m.p2[i] << " ";
        }
        os << endl;

        os << setw(2) << m.p2_store
           << " " << "==================" << " " 
           << setw(-2) << m.p1_store 
           << endl;

        os << "   ";
        for(auto i: m.p1){
            os << setw(2) << i << " ";
        }
        os << endl;

        return os;
    }
};

int main(){
    Mancala mancala;
        
    cout << mancala << endl;

    while(true){
        cout << "next turn ? [" << mancala.current_turn() << "]" << endl;
        int move;
        cin >> move;

        if(move == -1) break;

        mancala.play_turn(move);
        cout << mancala << endl;

        if(mancala.win() != -1){
            cout << mancala.win() << " wins" << endl;
            break;
        }
    }

    return 0;
}