#ifndef RESULTITEM_H
#define RESULTITEM_H

#include <QPainter>

#include <iostream>

using namespace std;

class ResultItem
{    
public:
    ResultItem(int width, int height, QString label="");
    int width, height, x, y;
    void render(QPainter *painter, int x, int y);

    QString label;

    bool isInside(int x, int y);
};

#endif //RESULTITEM_H
