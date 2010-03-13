#ifndef BUTTON2D_H
#define BUTTON2D_H

#include <QString>
#include <QPainter>
#include <iostream>

using namespace std;

class Button2D
{
public:
    Button2D(QString buttonLabel = "", int x = 0, int y = 0,
             int width = 0, int height = 0, bool background = false);

    QString buttonLabel;
    int x, y, width, height;

    int bx, by, bwidth, bheight;

    bool selected;

    //=true, if button  should have a background (rectangle, surrounding the button)
    bool background;

    int spacing;

    //works only for true circle
    bool isInside(int x, int y);

    void render(QPainter *painter);

    void resize(int x, int y, int width, int height);

    //if inside container, should be called after adding to this container
    //inside container's initialize function
    void calculateInnerPositions();

    //text stuff
    int y_text;
    int x_text;

    int height_text;
    int width_text;

    QFont buttonFont;
};

#endif //BUTTON2D_H
