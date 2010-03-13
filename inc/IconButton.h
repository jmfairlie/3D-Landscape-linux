#ifndef ICONBUTTON_H
#define ICONBUTTON_H

#include <QString>
#include <QPainter>
#include <iostream>

using namespace std;

class IconButton
{
public:

    IconButton(QString imagePath, QString buttonLabel = "", int x = 0, int y = 0,
             int width = 0, int height = 0);

    QString buttonLabel;
    int x, y, width, height;

    bool selected;

    int spacing;

    QImage img;

    bool isInside(int x, int y);

    void render(QPainter *painter);

    void resize(int x, int y, int width, int height);

    //if inside container, should be called after adding to this container
    //inside container's initialize function
    void calculateInnerPositions();

    //image stuff
    int x_image;
    int y_image;
    int width_image;
    int height_image;

    //text stuff
    int y_text;
    int x_text;
    int height_text;
    int width_text;

    QFont buttonFont;
};

#endif //ICONBUTTON_H
