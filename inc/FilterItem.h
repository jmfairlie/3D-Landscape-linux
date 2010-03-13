#ifndef FILTERITEM_H
#define FILTERITEM_H

#include "definesall.h"
#include <QPainter>

class FilterItem
{
public:
    FilterItem(QString label="", int x=0, int y=0, int w=0, int h=0);

    QString label;
    int x, y, width, height;

    bool isInside(int x, int y);

    //if inside container, should be called after adding to this container
    //inside container's initialize function
    void calculateInnerPositions();

    void render(QPainter *painter);

    //returns true, if cross was clicked
    bool crossClicked(int x, int y);

    bool wasMouseHover;

    void setTextFont();

    int spacing;

    int h_circle;
    int w_circle;
    int x_circle;
    int y_circle;

    int y_text;
    int x_text;
    int height_text;
    int width_text;

    //bounding rectangle for the cross
    int y_rect;
    int x_rect;
    int height_rect;
    int width_rect;

    QFont textFont;
};

#endif //FILTERITEM_H
