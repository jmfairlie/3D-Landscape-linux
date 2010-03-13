#ifndef SLIDER2D_H
#define SLIDER2D_H

#include <QPainter>

#include "definesall.h"
#include <cmath>

#include <iostream>

using namespace std;

class Slider2D
{
public:
    Slider2D(QString label = 0, int rangeStart = 0, int rangeEnd = 255,
             int x = 0, int y = 0,
             int width = 0, int height = 0
             );

    int x, y;

    int width, height;

    int spacing;

    bool selected;

    //first point
    int x1, y1;
    //second point
    int x2, y2;
    //slider pos
    int sx, sy;
    //slider size
    int swidth, sheight;

    QString label;
    int rangeStart, rangeEnd;

    QFont valueFont;
    QFont titleFont;

    int title_size;

    //if it is inside the rectangle, which bounds slider
    bool isInside(int x, int y);

    //parameter - x coordinate of mouse event
    //this function should be called only if
    //isInside returned true
    void updateValue(int x);

    void render(QPainter *painter);

    //inside container's initialize function
    void calculateInnerPositions();

    //position and size of title
    int x_label, y_label, width_label, height_label;
    //position and size of title
    int x_value, y_value, width_value, height_value;

    QString value;

    QRect boundSliderR;
};

#endif
