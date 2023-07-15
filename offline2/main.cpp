#include<iostream>
#include<iomanip>
#include<vector>
#include<numeric>
#include<functional>

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
            if(p1_store == p2_store) return 0;
            return p1_store > p2_store? 1: 2;
        }
        return -1;
    }

    int current_turn(){
        if(is_game_over) return -1;
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

    vector<int> get_available_moves(){
        vector<int>& v = getPlayerVector('s');
        vector<int> ans;

        for(int i = 0; i < v.size(); i++){
            if(v[i] != 0) ans.push_back(i);
        }
        return ans;
    }

    int get_p1_score(){ return p1_store;}
    int get_p2_score(){ return p2_store;}


    friend int heuristic1(Mancala &mancala, int me);
};

int heuristic1(Mancala &mancala, int me){
    if(me == 1){
        return mancala.p1_store - mancala.p2_store;
    }else{
        return mancala.p2_store - mancala.p1_store;
    }
}

class AlphaBetaPlayer{
    const int maximize;
    function<int(Mancala &, int)> heuristic;

    int _suggest_next_move(
        Mancala &mancala, int depth, 
        int alpha, int beta
    ){
        if(depth == 0 || mancala.win()){
            return heuristic(mancala, maximize);
        }

        if(mancala.current_turn() == maximize){
            int value = INT32_MIN;

            vector<int> next_moves = mancala.get_available_moves();
            for(auto i: next_moves){
                Mancala temp(mancala);
                temp.play_turn(i);
                int node_value = _suggest_next_move(temp, depth-1, alpha, beta);

                value = max(node_value, value);

                if(value >= beta){
                    break;
                }
                alpha = max(alpha, value);
            }
            return value;
        }else{
            int value = INT32_MAX;
            vector<int> next_moves = mancala.get_available_moves();
            for(auto i: next_moves){
                Mancala temp(mancala);
                temp.play_turn(i);
                int node_value = _suggest_next_move(temp, depth-1, alpha, beta);
                value = min(node_value, value);

                if(value <= alpha){
                    break;
                }
                beta = min(beta, value);
            }
            return value;
        }

        cerr << "ERROR : " << __LINE__ << endl;
        return 0;
    }

public:
    AlphaBetaPlayer(int maximize, function<int(Mancala &, int)> heuristic)
        : maximize(maximize), heuristic(heuristic)
    {

    }

    int suggest_next_move(Mancala &mancala, int depth){
        int alpha = INT32_MIN;
        int beta = INT32_MAX;
        int value = INT32_MIN;

        int move = -1;

        vector<int> next_moves = mancala.get_available_moves();
        
        for(auto i: next_moves){
            Mancala temp(mancala);
            temp.play_turn(i);
            int node_value = _suggest_next_move(temp, depth-1, alpha, beta);

            if(node_value > value){
                move = i;
            }
            value = max(node_value, value);

            // value always less than beta
            // if(value >= beta){
            //     break;
            // }
            alpha = max(alpha, value);
        }
        return move;
    }
};

void manVsbot(){
    Mancala mancala;
    AlphaBetaPlayer player1(2, heuristic1);
    
        
    cout << mancala << endl;

    while(true){
        int move;
        cout << "next turn ? [" << mancala.current_turn() << "]" << endl;
        if(mancala.current_turn() == 2){
            move = player1.suggest_next_move(mancala, 100);
            cout << "bot played [ " << move << " ]" << endl;  
        }
        else{
            cout << "Your turn : " ;
            cin >> move;
        }

        if(move == -1) break;

        mancala.play_turn(move);
        cout << mancala << endl;

        if(mancala.win() != -1){
            cout << mancala.win() << " wins" << endl;
            break;
        }
    }
}

int main(){
    Mancala mancala;
    AlphaBetaPlayer player1(1, heuristic1);
    AlphaBetaPlayer player2(2, heuristic1);

    while(true){
        if(mancala.current_turn() == 1){
            mancala.play_turn(player1.suggest_next_move(mancala, 100));
        }else if(mancala.current_turn() == 2){
            mancala.play_turn(player2.suggest_next_move(mancala, 100));
        }else{
            if(mancala.win() != -1){
                break;
            }
        }
    }
    cout << mancala.get_p1_score() << " " << mancala.get_p2_score() << endl;

    return 0;
}