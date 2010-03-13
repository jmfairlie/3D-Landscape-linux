#ifndef EDITITEM_H
#define EDITITEM_H

#include <QString>
#include <QPainter>

#include "definesall.h"

#include <iostream>

#include <QKeyEvent>

using namespace std;

class EditItem
{
public:
    EditItem(QString label="", int x=0, int y=0, int w=0, int h=0, int maxLen=255);

    QString label;
    int x, y, width, height;

    QString currText;
    int maxLen;

    bool hasFocus;

    bool isInside(int x, int y);

    //if inside container, should be called after adding to this container
    //inside container's initialize function
    void calculateInnerPositions();

    void render(QPainter *painter);

    void processKeyEvent(QKeyEvent *e);

    void setTextFont();
    
    int spacing;

    int h_inner;
    int w_inner;
    int x_inner;
    int y_inner;

    int y_text;
    int x_text;
    int height_text;
    int width_text;

    int y_title;
    int x_title;
    int height_title;
    int width_title;

    int title_size;

    QFont titleFont;
    QFont textFont;
};

#endif //EDITITEM_H
