#ifndef CHECKBOX_ITEM_H
#define CHECKBOX_ITEM_H

#include <QString>
#include <QPainter>
#include <QFont>

#include "definesall.h"
#include <iostream>

using namespace std;

class CheckBoxItem
{
public:
    CheckBoxItem(QString label = "", int x = 0, int y = 0, int w = 0, int h = 0);

    int x, y, width, height;
    QString label;

    bool checked;

    int spacing;

    bool isInside(int x, int y);

    //if inside container, should be called after adding to this container
    //inside container's initialize function
    void calculateInnerPositions();

    void render(QPainter *painter);

    int h_inner;
    int w_inner;
    int x_inner;
    int y_inner;

    int y_text;
    int x_text;
    int height_text;
    int width_text;

    QFont itemFont;
};

#endif //CHECKBOX_ITEM_H
