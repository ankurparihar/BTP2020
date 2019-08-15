#include <bits/stdc++.h>
#include <string>
using namespace std;

class Mobile_Station{
public:
    string name;
    pair<int,int> points;
    void* towerName;
};
class Base_Station{
public:
    string name;
    pair<int,int> points;
    vector<void*> mobileName;
};
int main(){
    Base_Station tower[10];
    Mobile_Station cellPhones[100];
    for(int i=0;i<10;i++){
        int x,y;
        x=(rand()%100)+1;
        y=(rand()%100)+1;
        string s="Tower";
        s+=to_string(i);
        tower[i].name=s;
        tower[i].points.first=x;
        tower[i].points.second=y;
        cout<<&tower[i]<<" "<<tower[i].name<<" - "<<x<<" "<<y<<endl;
    }
    cout<<endl;
    for(int i=0;i<100;i++){
        int x,y;
        x=(rand()%100)+1;
        y=(rand()%100)+1;
        string s="Mobile";
        s+=to_string(i);
        cellPhones[i].name=s;
        cellPhones[i].points.first=x;
        cellPhones[i].points.second=y;
        cout<<&cellPhones[i]<<" "<<cellPhones[i].name<<" - "<<x<<" "<<y<<endl;
    }
    cout<<endl;
    for(int i=0;i<100;i++){
        void* temp;
        double dist=100000000.0;
        for(int j=0;j<10;j++){
            double tempDist=sqrt(pow(cellPhones[i].points.first-tower[j].points.first,2)+
                                 pow(cellPhones[i].points.second-tower[j].points.second,2)*1.0);
            if(tempDist<dist){
                temp=(Base_Station*)&tower[j];
                dist=tempDist;
            }
        }
        cellPhones[i].towerName=temp;
        (*(Base_Station*)cellPhones[i].towerName).mobileName.push_back(&cellPhones[i]);
        cout<<cellPhones[i].name<<" "<<(*(Base_Station*)cellPhones[i].towerName).name<<endl;
    }
    cout<<endl;
    for(int i=0;i<10;i++){
        cout<<tower[i].name<<" - ";
        for(int j=0;j<tower[i].mobileName.size();j++)
            cout<<(*(Mobile_Station*)tower[i].mobileName[j]).name<<" ";
        cout<<endl;
    }
    return 0;
}
