#ifndef FILTERBOX_H
#define FILTERBOX_H

#include "FilterItem.h"

#include <QMouseEvent>
#include <cmath>
#include <iostream>

using namespace std;

class FilterBox
{
public:
    FilterBox();
    //layout - 0 = vertical, 1 = horizontal
    FilterBox(int x, int y, int width, int height,
                  int layout = 1, QString title = "");

    virtual ~FilterBox();

    void initialize();

    int spacing;

    int title_size;

    QList<FilterItem*> items;

    int x, y, width, height, layout;

    QString processMouseEvent(QMouseEvent *e);

    void render(QPainter *painter);

    void resize(int x, int y, int width, int height);

    QString title;

    //title text rectangle
    QRect titleTextR;
};

#endif //FILTERBOX_H
