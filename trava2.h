#ifndef TRAVA2_H
#define TRAVA2_H
#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <unistd.h> //для unix
#include <cstdlib>
using namespace std;
const int xl=50,yl=50; //размер
const int pymin=10,pymax=20,hymin=10,hymax=20; //границы возраста
const int pEat=2,hEat=2,EatCount=5; //еда сичтается за EatCount ходов, pEat и hEat - коэфициенты еды, нужной для жизни
const int nP=50; //начальное количество хищников
const int nH=50; //начальное кол-во травоядных
const int hChild=3, pChild=3; //сколько еды нужно, чтобы родить ребенка
const int hReproduce[2]={3,6},pReproduce[2]={3,10}; //репродуктивные границы
const int hMaxChild=4,pMaxChild=2; //максимальное количество детей за 1 раз
const int nTrava=300; // кол-во травы;
const int tPercent=20; //процент восстановления травы за 1 ход
const int nFire=5; //кол-во пожаров за ход
const int MeteoriteChance=10000; //шанс метеорита 1/n
const int pSickChance=150, hSickChance=150; //шанс заболеть 1/n
const int phN=10;
class Trava{
public:
    int recoveryCount=100,y=arc4random()%yl,x=arc4random()%xl;
    bool dead = false;
    void recovery(){
        if(recoveryCount<100){
            recoveryCount+=tPercent;
        }
    }
    char visual(){
        if(recoveryCount<100) return '_';
        return '^';
    }
};

class PredatorHerb{
public:
    int x=arc4random()%xl,y=arc4random()%yl;

};
class Predator{
public:
    int x=arc4random()%xl,y=arc4random()%yl,year{}, maxyear=(arc4random())%(pymax-pymin)+pymin,child=0,bufcount=0;
    vector<int> food={0,0};
    vector < vector <int> > buf={{}};
    bool dead = false,sick=false;
    void life(){
        for(int cock=0;cock<1;cock++){
            if(year==maxyear or food.size()<(pEat/*year)/pymin*/)){
                dead=true;
                break;
            }

            for(int i=0;i<food.size();i++){
                food[i]++;
            }
            vector<int> odd;
            copy_if(food.begin(), food.end(), back_inserter(odd), [](int i) {
                return i<EatCount == 1;
            });
            food=odd;

            if(year<pReproduce[1] && year>pReproduce[0] && pChild<=food.size() && !sick){
                child=arc4random()%pMaxChild;
            }
            if(arc4random()%pSickChance==1)
                sick= true;

        }}
    int ym(int y,int r){
        if(r==0){
            if(y==0){
                y+=1;
            } else if(y==yl-1){
                y-=1;
            } else {
                if(arc4random()%2==0)
                    y+=1;
                else
                    y-=1;
            }
        } else {
            if(y==0){
                y+=arc4random()%2;
            } else if(y==yl-1){
                y-=arc4random()%2;
            } else
                y+=arc4random()%3-1;

        }
        return y;
    }
    int xm(int x){
        if(x==0){
            x+=arc4random()%2;
        } else if(x==xl-1){
            x-=arc4random()%2;
        } else {
            x+=arc4random()%2-1;
        }
        return x;
    }
    void search(int xh, int yh){
        if(!sick){
            buf.push_back({xh,yh});
            bufcount++;}
    }
    void move(){
        int bufx=-1, bufy=-1;
        year++;
        float smin=1000000,s;
        for(int i=1;i<bufcount;i++){
            try{
                s=sqrt(pow(x-buf[i][0],2)+pow(y-buf[i][1],2));}
            catch (...){
                s=1000;
            }
            if(s<smin){
                //cout<<s<<endl;
                smin=s;
                bufx=buf[i][0];
                bufy=buf[i][1];

            }
        }
        if(bufx==-1 || smin==0){
            int r=xm(x)-x;
            y=ym(y,r);
            x+=r;
            year++;
        } else {
            if(bufx>x)
                x++;
            else if(bufx<x)
                x--;

            if(bufy>y)
                y++;
            else if(bufy<y)
                y--;
        }
        buf={{}};
        bufcount=0;
    }
    bool eat(int xz,int yz){
        if(!sick){
            if( abs(x-xz)<2 && abs(y-yz)<2){
                food.push_back(0);
                return true;
            }}
        return false;
    }
};
class Herbivore{
public:
    int x=arc4random()%xl,y=arc4random()%yl,year, maxyear=(arc4random())%(hymax-hymin)+hymin,child=0,bufcount=0;
    vector<int> food={0,0};
    vector < vector <int> > buf={{}};
    bool dead = false, sick=false;
    int ym(int y,int r){
        if(r==0){
            if(y==0){
                y+=1;
            } else if(y==yl-1){
                y-=1;
            } else {
                if(arc4random()%2==0)
                    y+=1;
                else
                    y-=1;
            }
        } else {
            if(y==0){
                y+=arc4random()%2;
            } else if(y==yl-1){
                y-=arc4random()%2;
            } else
                y+=arc4random()%3-1;

        }
        return y;
    }
    int xm(int x){
        if(x==0){
            x+=arc4random()%2;
        } else if(x==xl-1){
            x-=arc4random()%2;
        } else {
            x+=arc4random()%2-1;
        }
        return x;
    }
    void life(){
        for(int cock=0;cock<1;cock++){
            if(year==maxyear || food.size()<(hEat/*year*/)/*hymin*/){
                dead=true;
                break;
            }

            for(int i=0;i<food.size();i++){
                food[i]++;
            }
            vector<int> odd;
            copy_if(food.begin(), food.end(), back_inserter(odd), [](int i) {
                return i<EatCount == 1;
            });
            food=odd;

            if(year<hReproduce[1] && year>hReproduce[0] && hChild<=food.size() && !sick){
                child=arc4random()%hMaxChild;
            }
            if(arc4random()%hSickChance==1){
                sick= true;
            }
        }
    }

    void search(int xh, int yh){
        if(!sick){
            buf.push_back({xh,yh});
            bufcount++;
        }
    }
    void move(){
        int bufx=-1, bufy=-1;
        year++;
        float smin=1000000,s;
        for(int i=1;i<bufcount-1;i++){
            try{
                s=sqrt(pow(x-buf[i][0],2)+pow(y-buf[i][1],2));}
            catch (...){
                s=1000;
            }
            if(s<smin){
                //cout<<s<<endl;
                smin=s;
                bufx=buf[i][0];
                bufy=buf[i][1];

            }

        }
        if(bufx==-1 || smin==0){
            int r=xm(x)-x;
            y=ym(y,r);
            x+=r;
            year++;
        } else {
            if(bufx>x)
                x++;
            else if(bufx<x)
                x--;

            if(bufy>y)
                y++;
            else if(bufy<y)
                y--;
        }
        buf={{}};
        bufcount=0;
    }

    bool eat(int xz,int yz){
        if(!sick){
            if(x==xz and y==yz){
                food.push_back(0);
                return true;
            }}
        return false;

    }

};

class Fire{
public:
    int x=arc4random()%xl,y=arc4random()%yl;
};


#endif // TRAVA2_H
