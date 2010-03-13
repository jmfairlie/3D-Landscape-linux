#ifndef DISTANCESELECTOR_H
#define DISTANCESELECTOR_H

#include "RadioButton2D.h"
//#include "Button2D.h"
#include "IconButton.h"
#include "Dialog2D.h"

#include <QPainter>
#include <QMouseEvent>
#include <cmath>

class DistanceSelector : public Dialog2D
{
public:
    DistanceSelector(QSize mainWidgetSize);

    virtual ~DistanceSelector();

    void createRadioButton();

    RadioButton2D *radioButton;

    void render(QPainter *painter);

    void processMouseEvent(QMouseEvent *e);

    void resize(QSize mainWidgetSize);

    bool mouseActive;

    IconButton *bok, *bcancel;

    int circleCentreX, circleCentreY;
    int circleEdgeX, circleEdgeY;

    virtual void resetDialogState();

    int radius;

    QSize mainWidgetSize;
};

#endif //DISTANCESELECTOR_H
