#ifndef SLIDERBOX_H
#define SLIDERBOX_H

#include "Slider2D.h"
#include <QPainter>
#include <QList>
#include <QMouseEvent>

class SliderBox
{
public:
    SliderBox();
    //layout - 0 = vertical, 1 = horizontal
    SliderBox(int x, int y, int width, int height,
                  int layout = 1, QString title = "");

    virtual ~SliderBox();

    void initialize();

    int spacing;

    int title_size;

    QList<Slider2D*> items;

    int x, y, width, height, layout;

    void processMouseEvent(QMouseEvent *e);

    void render(QPainter *painter);

    void resize(int x, int y, int width, int height);

    QString title;

    //title text rectangle
    QRect titleTextR;
};

#endif //SLIDERBOX_H
