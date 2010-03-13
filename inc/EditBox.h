#ifndef EDITBOX_H
#define EDITBOX_H

#include <QMouseEvent>
#include <QKeyEvent>

#include "EditItem.h"

class EditBox
{
public:
    EditBox();
    //layout - 0 = vertical, 1 = horizontal
    EditBox(int x, int y, int width, int height,
                  int layout = 1, QString title = "");

    virtual ~EditBox();

    void initialize();

    int spacing;

    int title_size;

    QList<EditItem*> items;

    int x, y, width, height, layout;

    void processMouseEvent(QMouseEvent *e);
    void processKeyEvent(QKeyEvent *e);

    void render(QPainter *painter);

    void resize(int x, int y, int width, int height);

    QString title;

    //title text rectangle
    QRect titleTextR;
};

#endif //EDITBOX_H
