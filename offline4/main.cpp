#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include<set>
#include<assert.h>
#include<algorithm>
#include<numeric>
#include<cmath>
#include<cstdio>

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

class DecisionTree;

class Node
{
private:
    int split_attribute;
    int answer;
    vector<Node *> childs;
public:
    Node(int split, int answer = -1){
        split_attribute = split;
        this->answer = answer;
    }

    void addChild(Node * child){
        childs.push_back(child);
    }

    void setAnswer(int val){
        answer = val;
    }

    void print(int tab=0, int index=0){
        cout << string(tab, '\t') << index << " -> " << split_attribute << " " << answer << endl;

        int idx = 0;
        for(auto i: childs){
            i->print(tab+1, idx++);
        }
    }

    ~Node(){
        for(auto i: childs){
            delete i;
        }
    }

    friend DecisionTree;
};

vector<int> getClassCount(vector<Car> &cars){
    vector<int> class_count(N_CLASS,0);
    for(auto &i: cars){
        class_count[i.getValue(N_ATTR)]++;
    }

    return class_count;
}

class DecisionTree
{
private:

    double calculate_entropy(vector<Car> &cars){
        vector<int> class_count = getClassCount(cars);
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

    int find_best_split(vector<Car> cars){
        vector<int> attr_used(N_ATTR, 0);

        int updated = 0;
        double ent =  calculate_entropy(cars);
        double mx_gain = 0;
        double mx_idx = 0;

        for(int i = 0; i < attr_used.size(); i++){
            if(!attr_used[i]){
                updated = 1;
                double reminder = calculate_reminder(cars, i);
                double gain = ent - reminder;
                // cout << i << " " << gain << " " << reminder << endl;
                if(gain > mx_gain){
                    mx_gain = gain;
                    mx_idx = i;
                }
            }
        }

        // cout << mx_idx << ' ' << mx_gain << endl;
        return updated==0? -1: mx_idx;
    }

    int find_if_single_class(vector<Car> &cars){
        vector<int> class_cnt = getClassCount(cars);
        for(int i = 0; i < class_cnt.size(); i++){
            if(class_cnt[i] == cars.size()) return i;
        }
        return -1;
    }

    int find_majority(vector<Car> &cars){
        vector<int> class_cnt = getClassCount(cars);
        int mx_idx = max_element(class_cnt.begin(), class_cnt.end()) - class_cnt.begin();
        return mx_idx;
    }

    Node* train_helper(vector<Car> cars, int majority){
        if(cars.size() == 0) return new Node(-1, majority);

        // check if only one class
        int if_one_class= find_if_single_class(cars);
        // cout << if_one_class << endl;
        if(if_one_class != -1){
            return new Node(-2, if_one_class);
        }

        // get best split attribute
        int split_at = find_best_split(cars);
        
        // if no attribute left
        int majority_class = find_majority(cars);
        if(split_at == -1) return new Node(-3, majority_class);

        Node * node = new Node(split_at);
        for(int i = 0; i < 4; i++){
            vector<Car> childCars;
            for(auto j: cars){
                if(j.getValue(split_at) == i) childCars.push_back(j);
            }

            // if(childCars.size() == 0) continue;

            Node* child = train_helper(childCars, majority_class);
            node->addChild(child);
        }        

        return node;
    }

    Node* root;
public:
    DecisionTree() {
        root = nullptr;
    }

    void train(vector<Car> cars){
        root = train_helper(cars, -1);
    }

    int test(Car &car){
        auto cur = root;

        while(cur){
            int split = cur->split_attribute;
            if(split < 0){
                return cur->answer;
            }

            int attr_value = car.getValue(split);
            // cout << attr_value << " " << split << " " << cur->childs.size()  << endl;
            cur = cur->childs[attr_value];
        }

        return -1;
    }

    vector<int> test(vector<Car> &cars){
        vector<int> ans;

        for(auto &i: cars){
            ans.push_back(test(i));
        }

        return ans;
    }


    ~DecisionTree() {
        if(root != nullptr) delete root;
    }
};

int main(){
    srand(1927);
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

    vector<Car> train, test;
    for(auto i: cars){
        int random = rand() % 10;
        if(random >= 8) test.push_back(i);
        else train.push_back(i);
    }

    vector<int> total_class = getClassCount(cars);
    vector<int> train_class = getClassCount(train);
    vector<int> test_class = getClassCount(test);

    for(auto i: total_class) cout << i << "\t" ; cout << endl;
    for(auto i: train_class) cout << i << "\t" ; cout << endl;
    for(auto i: test_class) cout << i << "\t" ; cout << endl;

    DecisionTree tree;
    tree.train(train);

    vector<int> ans = tree.test(test);
    int match = 0;
    for(int i = 0; i < ans.size(); i++){
        match += test[i].getValue(N_ATTR) == ans[i];
    }
    cout << match << " " << ans.size() << " " << (double)match / ans.size() << endl;     
    return 0;
}