#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "CheckBoxItem.h"
#include <QPainter>
#include <QList>
#include <QMouseEvent>

class CheckBox
{
public:
    CheckBox();
    //layout - 0 = vertical, 1 = horizontal
    CheckBox(int x, int y, int width, int height,
                  int layout = 1, QString title = "");

    virtual ~CheckBox();

    void initialize();

    int spacing;

    int title_size;

    QList<CheckBoxItem*> items;

    int x, y, width, height, layout;

    void processMouseEvent(QMouseEvent *e);

    void render(QPainter *painter);

    void resize(int x, int y, int width, int height);

    QString title;

    //title text rectangle
    QRect titleTextR;
};

#endif //CHECKBOX_H
