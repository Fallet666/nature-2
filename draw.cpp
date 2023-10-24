#include "draw.h"
//#include "trava2.h"
#include "./ui_draw.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
#include <vector>
#include <random>
//#include <unistd.h> //для unix
#include <cstdlib>
#include <QCoreApplication>
#include <QPair>
#include <QTime>
#include <QEventLoop>
#include <QTimerEvent>
#include <QFont>
#include <QRect>

using namespace std;


draw::draw(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::draw)
{
    ui->setupUi(this);
    setlocale(0,"");
    mainCount=0;
    int n1=nH,n=nP;
    totalH=n1,totalP=nP;
    totalr=55;
    ten=10;
    startTimer(1000);
    int count=0;
    while(count<nTrava){
        bool t=true;
        Trava a;
        tArr.push_back(a);
        for(int i=0;i<count;i++){
            if(tArr[i].y==tArr[count].y && tArr[i].x==tArr[count].x)
                t=false;
        }
        if(!t)
            tArr[count].x=arc4random()%xl,tArr[count].y=arc4random()%yl;
        else
            count++;
    } //ok
    count=0;
    while(count<phN){
        bool t=true;
        PredatorHerb a;
        phArr.push_back(a);
        for(int i=0;i<count;i++){
            if(phArr[i].y==phArr[count].y && phArr[i].x==phArr[count].x)
                t=false;
        }
        if(!t)
            phArr[count].x=arc4random()%xl,phArr[count].y=arc4random()%yl;
        else
            count++;
    }
    int mainCount=0;
    for(int i=0;i<n;i++){
        Predator a;
        pArr.push_back(a);
    }
    for(int i=0;i<n1;i++){
        Herbivore a;
        hArr.push_back(a);
    }
}

draw::~draw()
{
    delete ui;
}

vector<Trava> draw::filter(vector<Trava> arr){
    vector<Trava> buf;
    for(Trava t: arr){
        if(!t.dead){
            buf.push_back(t);
        }
    }
    return buf;
}
vector<Predator> draw::filterP(vector<Predator> arr){
    vector<Predator> buf;
    for(Predator t: arr){
        if(!t.dead){
            buf.push_back(t);
        }
    }
    return buf;
}
vector<Herbivore> draw::filterH(vector<Herbivore> arr){
    vector<Herbivore> buf;
    for(Herbivore t: arr){
        if(!t.dead){
            buf.push_back(t);
        }
    }
    return buf;
}
vector<int> draw::poisk(int x,int y, vector<string> s,char goal){
    vector<int> arr;
    for(int i=1;i<xl;i++){
        if(x-i>-1){
            for(int j=y-i;j<=y+i;j++){
                if(j>-1 && j<yl){
                    if(s[j][x-i]==goal){
                        arr={x-i,j};
                        return arr;
                    }
                }
            }}
        if(x+i<xl){
            for(int j=y-i;j<=y+i;j++){
                if(j>-1 && j<yl){
                    if(s[j][x-i]==goal){
                        arr={x-i,j};
                        return arr;
                    }
                }
            }}
        if(y-i>-1){
            for(int j=x-i+1;j<x+i;j++){
                if(j>-1 && j<yl){
                    if(s[y-i][j]==goal){
                        arr={j,y-i};
                        return arr;
                    }
                }
            }}
        if(y+i>-1){
            for(int j=x-i+1;j<x+i;j++){
                if(j>-1 && j<yl){
                    if(s[y+i][j]==goal){
                        arr={j,y+i};
                        return arr;
                    }
                }
            }}
    }
    arr={-1,-1};
    return arr;
}
void draw::timerEvent(QTimerEvent *)
{
//    update();
}
void draw::paintEvent(QPaintEvent *)
{   if(tcount1+tcount2!=0){
    QPainter paint(this);
    paint.setPen(QPen(Qt::white,5,Qt::SolidLine,Qt::RoundCap));
    paint.drawRect(0,0,505,505);
    int hcount = 0, pcount = 0, hsick = 0, psick = 0;
    tcount1 = 0, tcount2 = 0;
    vector<string> f(yl, string(xl + 1, ' '));
    tArr = filter(tArr);
    for (int i = 0; i < tArr.size(); i++) { //trava
        if (!tArr[i].dead) {
            tArr[i].recovery();
            f[tArr[i].y][tArr[i].x] = tArr[i].visual();
            if (f[tArr[i].y][tArr[i].x] == '_')
                tcount2++;
            else
                tcount1++;
        }
    }
    for (int i = 0; i < phN; i++) { //хищная трава
        f[phArr[i].y][phArr[i].x] = '7';
    }
    hArr=filterH(hArr);
    for (int i = 0; i < hArr.size(); i++) { //herd
            hcount++;
            if (hArr[i].sick)
                hsick++;
            hArr[i].life();
            for (int q = 0; q < tArr.size(); q++) {
                if (!tArr[q].dead && tArr[q].recoveryCount>=100) {
                    hArr[i].search(tArr[q].x, tArr[q].y);
                }
            }
            hArr[i].move();
            f[hArr[i].y][hArr[i].x] = '%';
            for (int q = 0; q < tArr.size(); q++) {
                if (tArr[q].recoveryCount == 100 && !tArr[q].dead) {
                    if (hArr[i].eat(tArr[q].x, tArr[q].y)) {
                        tArr[q].recoveryCount = 0;
                    }
                }
            }
            for(int j=0;j< hArr[i].child;j++){
                Herbivore a;
                hArr.push_back(a);
                totalH++;
            }
        }
    pArr=filterP(pArr);
    for (int i = 0; i < pArr.size(); i++) { //pred
            pcount++;
            if (pArr[i].sick) {
                psick++;
            }
            pArr[i].life();
            for (int q = 0; q < hArr.size(); q++) {
                if (!hArr[q].dead) {
                    if (pArr[i].eat(hArr[q].x, hArr[q].y)) {
                        hArr[q].dead = true;
                    }
                }
            }
            for (int q = 0; q < hArr.size(); q++) {
                if (!hArr[q].dead) {
                    pArr[i].search(hArr[q].x, hArr[q].y);
                }
            }
            pArr[i].move();
            f[pArr[i].y][pArr[i].x] = '*';
            for(int j=0;j<pArr[i].child;j++){
                Predator a;
                pArr.push_back(a);
                totalP++;
            }

    }
    Fire fire[nFire];
    int bufcount = 0;
    for (int y = 0; y < nFire; y++) { //Fire and PH

        f[fire[y].y][fire[y].x] = '#';
        for (int i = 0; i < tArr.size(); i++) {
            if (bufcount < phN) {
                if (phArr[bufcount].y == tArr[i].y && phArr[bufcount].x == tArr[i].x)
                    tArr[i].dead = true;
            }
            if (fire[y].y == tArr[i].y && fire[y].x == tArr[i].x)
                tArr[i].dead = true;
        }
        for (int i = 0; i < pArr.size(); i++) {
            if (bufcount < phN) {
                if (phArr[bufcount].y == pArr[i].y && phArr[bufcount].x == pArr[i].x)
                    pArr[i].dead = true;
            }
            if (fire[y].y == pArr[i].y && fire[y].x == pArr[i].x)
                pArr[i].dead = true;
        }
        for (int i = 0; i < hArr.size(); i++) {
            if (bufcount < phN) {
                if (phArr[bufcount].y == hArr[i].y && phArr[bufcount].x == hArr[i].x)
                    hArr[i].dead = true;
            }
            if (fire[y].y == hArr[i].y && fire[y].x == hArr[i].x)
                hArr[i].dead = true;
        }
        bufcount++;
    }
    if (pcount == 0)
    {for(int i=0;i<50;i++){
            Predator a;
            pArr.push_back(a);

        }
        totalP+=50;}
    if (hcount == 0){
        for(int i=0;i<50;i++){
            Herbivore a;
            hArr.push_back(a);
        }
        totalH+=50;
    }
    for(int i=0;i<f.size();i++){
        for(int j=0;j<f[i].size();j++){
            if(f[i][j]=='_'){
                paint.setPen(QPen(Qt::gray,1,Qt::SolidLine,Qt::RoundCap));
                paint.drawLine(i*10+5,j*10+5,i*10+9,j*10+5);
            } else if(f[i][j]=='^'){
                paint.setPen(QPen(Qt::green,1,Qt::SolidLine,Qt::RoundCap));
                paint.drawLine(i*10+5,j*10+5,i*10+5,j*10+2);
                paint.setPen(QPen(Qt::green,1,Qt::SolidLine,Qt::RoundCap));
                paint.drawLine(i*10+6,j*10+5,i*10+5,j*10+1);
                paint.setPen(QPen(Qt::green,1,Qt::SolidLine,Qt::RoundCap));
                paint.drawLine(i*10+7,j*10+5,i*10+5,j*10+5);
                paint.setPen(QPen(Qt::green,1,Qt::SolidLine,Qt::RoundCap));
                paint.drawLine(i*10+8,j*10+5,i*10+5,j*10+6);
                paint.setPen(QPen(Qt::green,1,Qt::SolidLine,Qt::RoundCap));
                paint.drawLine(i*10+9,j*10+5,i*10+5,j*10+4);
            } else if(f[i][j]=='7'){
                paint.setPen(QPen(Qt::darkGreen,3,Qt::SolidLine,Qt::RoundCap));
                paint.drawRect(i*10+5,j*10+5,5,5);
                paint.setPen(QPen(Qt::blue,7,Qt::SolidLine,Qt::RoundCap));
                paint.drawPoint(i*10+8,j*10+8);
                paint.setPen(QPen(Qt::yellow,4,Qt::SolidLine,Qt::RoundCap));
                paint.drawPoint(i*10+8,j*10+8);
                paint.setPen(QPen(Qt::magenta,3,Qt::SolidLine,Qt::RoundCap));
                paint.drawPoint(i*10+8,j*10+8);
            } else if(f[i][j]=='%'){
                paint.setPen(QPen(Qt::white,3,Qt::SolidLine,Qt::RoundCap));
                paint.drawLine(i*10+5,j*10+5,i*10+9,j*10+5);
                paint.setPen(QPen(Qt::white,3,Qt::SolidLine,Qt::RoundCap));
                paint.drawPoint(i*10+12,j*10+3);
                paint.setPen(QPen(Qt::white,2,Qt::SolidLine,Qt::RoundCap));
                paint.drawLine(i*10+5,j*10+5,i*10+5,j*10+8);
                paint.setPen(QPen(Qt::white,2,Qt::SolidLine,Qt::RoundCap));
                paint.drawLine(i*10+9,j*10+5,i*10+9,j*10+8);

            } else if(f[i][j]=='*'){
                paint.setPen(QPen(Qt::darkGray,2,Qt::SolidLine,Qt::RoundCap));
                paint.drawLine(i*10+5,j*10+5,i*10+10,j*10+5);
                paint.setPen(QPen(Qt::darkGray,2,Qt::SolidLine,Qt::RoundCap));
                paint.drawLine(i*10+11,j*10+4,i*10+13,j*10+4);
                paint.setPen(QPen(Qt::darkGray,2,Qt::SolidLine,Qt::RoundCap));
                paint.drawLine(i*10+5,j*10+5,i*10+5,j*10+8);
                paint.setPen(QPen(Qt::darkGray,2,Qt::SolidLine,Qt::RoundCap));
                paint.drawLine(i*10+10,j*10+5,i*10+10,j*10+8);
                paint.setPen(QPen(Qt::darkGray,1,Qt::SolidLine,Qt::RoundCap));
                paint.drawLine(i*10+5,j*10+5,i*10+3,j*10+3);
            } else if(f[i][j]=='#'){
                paint.setPen(QPen(Qt::red,1,Qt::SolidLine,Qt::RoundCap));
                paint.drawLine(i*10+5,j*10+5,i*10+5,j*10+5);
                paint.setPen(QPen(Qt::red,1,Qt::SolidLine,Qt::RoundCap));
                paint.drawLine(i*10+6,j*10+5,i*10+5,j*10+1);
                paint.setPen(QPen(Qt::red,1,Qt::SolidLine,Qt::RoundCap));
                paint.drawLine(i*10+7,j*10+5,i*10+5,j*10+5);
                paint.setPen(QPen(Qt::red,1,Qt::SolidLine,Qt::RoundCap));
                paint.drawLine(i*10+8,j*10+5,i*10+5,j*10+2);
                paint.setPen(QPen(Qt::red,1,Qt::SolidLine,Qt::RoundCap));
                paint.drawLine(i*10+9,j*10+5,i*10+5,j*10+1);
            }
        }
    }
    mainCount++;
    paint.setPen(QPen(Qt::white,5,Qt::SolidLine,Qt::RoundCap));
    QFont font = paint.font();
    font.setPixelSize(14);
    paint.setFont(font);
    const QRect rectangle = QRect(520, 10, 1000, 50);
    QRect boundingRect;
    QString s="Хищники:"+QString::number(pcount);
    paint.drawText(rectangle, 0, s, &boundingRect);

    const QRect rectangle3 = QRect(520, 30, 1000, 50);
    s="Травоядные:"+QString::number(hcount);
    paint.drawText(rectangle3, 0, s, &boundingRect);

    const QRect rectangle4 = QRect(520, 60, 1000, 50);
    s="Норм трава:"+QString::number(tcount1);
    paint.drawText(rectangle4, 0, s, &boundingRect);

    const QRect rectangle5 = QRect(520, 80, 1000, 50);
    s="Съеденная трава:"+QString::number(tcount2);
    paint.drawText(rectangle5, 0, s, &boundingRect);

    font.setPixelSize(16);
    paint.setFont(font);

    paint.setPen(QPen(Qt::red,5,Qt::SolidLine,Qt::RoundCap));
    if(mainCount/ten){
        ten*=10;
        totalr+=8;
    }
    paint.drawRect(523,110,totalr,20);
    paint.setPen(QPen(Qt::white,5,Qt::SolidLine,Qt::RoundCap));
    const QRect rectangle6 = QRect(528, 110, 1000, 50);
    s="Шаг:"+QString::number(mainCount);
    paint.drawText(rectangle6, 0, s, &boundingRect);

    font.setPixelSize(14);
    paint.setFont(font);

    const QRect rectangle2 = QRect(520, 140, 1000, 50);
    QString s2="Хищники total:"+QString::number(totalP);
    paint.drawText(rectangle2, 0, s2, &boundingRect);

    const QRect rectangle8 = QRect(520, 160, 1000, 50);
    s="Травоядные total:"+QString::number(totalH);
    paint.drawText(rectangle8, 0, s, &boundingRect);
    update();} else {
    QPainter paint(this);
    paint.setPen(QPen(Qt::white,5,Qt::SolidLine,Qt::RoundCap));
    QRect abobus(0,0,505,505);
    paint.drawRect(abobus);
    QFont font = paint.font();
    font.setPixelSize(50);
    paint.setFont(font);
    paint.setPen(QPen(Qt::red,7,Qt::SolidLine,Qt::RoundCap));
    paint.drawText(abobus,Qt::AlignCenter, tr("The End"));


    }
}
void draw::delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}
void draw::clear(QPainter &paint)
{
}
