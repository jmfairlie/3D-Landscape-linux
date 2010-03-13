#ifndef RADIOBUTTON2D_H
#define RADIOBUTTON2D_H

#include "Button2D.h"
#include "definesall.h"
#include <QList>
#include <QMouseEvent>

class RadioButton2D
{
public:
    RadioButton2D();
    //layout - 0 = vertical, 1 = horizontal
    RadioButton2D(int x, int y, int width, int height,
                  int layout = 1);

    virtual ~RadioButton2D();

    void initialize();    

    void resize(int x, int y, int width, int height);
    bool isInside(int x, int y);

    QList<Button2D*> buttons;

    void render(QPainter *painter);

    void processMouseEvent(QMouseEvent *e);

    void resetSelections();

private:
    int x, y, width, height, layout, spacing;
};

#endif //RADIOBUTTON2D_H
