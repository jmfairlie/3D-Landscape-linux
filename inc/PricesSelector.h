#ifndef PRICESSELECTOR_H
#define PRICESSELECTOR_H

#include <QPainter>
#include <QMouseEvent>
#include <cmath>

#include "SliderBox.h"
//#include "Button2D.h"
#include "IconButton.h"
#include "Dialog2D.h"

class PricesSelector : public Dialog2D
{
public:
    PricesSelector(QSize mainWidgetSize);

    virtual ~PricesSelector();

    void createSliderBox();

    SliderBox *sliderBox;

    void render(QPainter *painter);

    void processMouseEvent(QMouseEvent *e);

    bool mouseActive;

    IconButton *okB, *cancelB;

    QSize mainWidgetSize;

    virtual void resetDialogState();

    void resize(QSize size);
};

#endif //PRICESSELECTOR
