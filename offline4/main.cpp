#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include<set>
#include<assert.h>
#include<algorithm>
#include<numeric>
#include<cmath>

using namespace std;

#define N_ATTR 6
#define N_CLASS 4

class Car
{
private:
    /* data */
    int buying, maint, doors, persons, lug_boot, safety;
    int classValue;

    int setBuying(string buying){
        if(buying == "vhigh") return 3;
        else if(buying == "high") return 2;
        else if(buying == "med") return 1;
        else if(buying == "low") return 0;
        else return -1;
    }

    int setLugboot(string lugboot){
        if(lugboot == "small" || lugboot == "low") return 0;
        else if(lugboot == "med") return 1;
        else if(lugboot == "big" || lugboot == "high") return 2;
        else return -1;
    }

    int setDoors(string doors){
        return stoi(doors.substr(0,1)) - 2;
    }

    int setPersons(string persons){
        if(persons == "more") return 2;
        else return stoi(persons) / 2 - 1;
    }

    int setClassValue(string value){
        if(value == "unacc") return 0;
        else if(value == "acc") return 1;
        else if(value == "good") return 2;
        else if(value == "vgood") return 3;
        else return -1;
    }
public:
    Car(){

    }

    Car(
        string buying, string maint,
        string doors, string persons,
        string lug_boot, string safety,
        string classValue
    ){
        this->buying = setBuying(buying);
        this->maint = setBuying(maint);
        this->doors = setDoors(doors);
        this->persons = setPersons(persons);
        this->lug_boot = setLugboot(lug_boot);
        this->safety = setLugboot(safety);
        this->classValue = setClassValue(classValue);
    }

    string getClassValue(){
        vector<string> v= {
            "", "unacc", "acc", "good", "vgood"
        };

        return v[classValue+1];
    }

    int getValue(int attr){
        switch (attr)
        {
        case 0:
            return buying;
            break;
        case 1:
            return maint;
            break;
        case 2:
            return doors;
            break;
        case 3:
            return persons;
            break;
        case 4:
            return lug_boot;
            break;
        case 5:
            return safety;
            break;
        case 6:
            return classValue;
            break;
        }

        return -1;
    }

};

class Node
{
private:
    int split_attribute;
    vector<Node *> childs;
public:
    Node(int split){
        split_attribute = split;
    }

    void addChild(Node * child){
        childs.push_back(child);
    }

    ~Node(){
        for(auto i: childs){
            i->~Node();
            delete i;
        }
    }
};

class DecisionTree
{
private:
    double calculate_entropy(vector<Car> &cars){
        vector<int> class_count(N_CLASS,0);
        for(auto &i: cars){
            class_count[i.getValue(N_ATTR)]++;
        }
        return calculate_B(class_count);
    }

    double calculate_B(vector<int> classwise_count){
        double entropy = 0;
        double sum = accumulate(classwise_count.begin(), classwise_count.end(), 0);
        for(auto i: classwise_count){
            if(i == 0) continue;
            double prob = i / sum;
            entropy += prob * log2(prob);
        }
        return -entropy;
    }
    
public:
    DecisionTree() {}

    void train(vector<Car> cars){
        vector<int> attr_used(N_ATTR, 0);

        double ent =  calculate_entropy(cars);
        double mx_gain = 0;
        double mx_idx = 0;

        for(int i = 0; i < attr_used.size(); i++){
            if(!attr_used[i]){
                double reminder = calculate_reminder(cars, i);
                double gain = ent - reminder;
                cout << i << " " << gain << " " << reminder << endl;
                if(gain > mx_gain){
                    mx_gain = gain;
                    mx_idx = i;
                }
            }
        }

        cout << mx_idx << ' ' << mx_gain << endl;
    }

    double calculate_reminder(vector<Car> &cars, int split){
        int table[4][N_CLASS] = {0};
        double sum_table[4] = {0};
        double total = (double) cars.size();

        for(auto i: cars){
            table[i.getValue(split)][i.getValue(N_ATTR)]++;
            sum_table[i.getValue(split)]++;
        }

        double remainder = 0;
        for(int i = 0; i < 4; i++){
            double class_p = sum_table[i] / total;
            double B = 0;
            for(int j = 0; j < N_CLASS; j++){
                if(table[i][j] == 0) continue;
                double proba = table[i][j] / sum_table[i];
                B += proba * log2(proba);
            }
            remainder += class_p * -B;
        }

        return remainder;
    }


    ~DecisionTree() {}
};

int main(){
    vector<Car> cars;

    // input
    string line;

    while (getline(cin, line)){
        stringstream s(line);
        string word;
        vector<string> v;
        while(getline(s, word, ',')){
            v.push_back(word);
        }

        assert(v.size() == N_ATTR + 1);
        cars.emplace_back(v[0], v[1], v[2], v[3], v[4], v[5], v[6]);
    }

    cout << cars.size() << endl;

    DecisionTree tree;
    tree.train(cars);
    
    return 0;
}