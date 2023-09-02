#include<iostream>
#include<vector>
#include<string>

using namespace std;

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

