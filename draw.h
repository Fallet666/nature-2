#ifndef DRAW_H
#define DRAW_H

#include <QtGui>
#include <math.h>
#include "trava2.h"
#include <QWidget>
#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <unistd.h> //для unix
#include <cstdlib>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class draw; }
QT_END_NAMESPACE

class draw : public QWidget
{
    Q_OBJECT

public:
    draw(QWidget *parent = nullptr);
    ~draw();
    void paintEvent(QPaintEvent *);
    void drawBase(QPainter &paint);
    void clear(QPainter &paint);
    void delay(int millisecondsToWait);
    void timerEvent(QTimerEvent *);
    vector<Trava> filter(vector<Trava> arr);
    vector<Predator> filterP(vector<Predator> arr);
    vector<Herbivore> filterH(vector<Herbivore> arr);
    vector<int> poisk(int x,int y, vector<string> s,char goal);
    int totalH,totalP;
    int totalr,ten;
    int tcount1 = -1, tcount2 = -1;
    int nT=nTrava;
    vector<Trava> tArr;
    vector<PredatorHerb> phArr;
    vector<Predator> pArr;
    vector<Herbivore> hArr;
    Fire fire[nFire];
    int n1,n;
    int mainCount;


private:
    Ui::draw *ui;
};

#endif // DRAW_H
