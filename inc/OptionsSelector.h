#ifndef OPTIONSSELECTOR_H
#define OPTIONSSELECTOR_H

#include <QPainter>
#include <QMouseEvent>
#include <cmath>

#include "CheckBox.h"
//#include "Button2D.h"
#include "IconButton.h"
#include "Dialog2D.h"

class OptionsSelector : public Dialog2D
{
public:
    OptionsSelector(QSize mainWidgetSize);

    virtual ~OptionsSelector();

    void createCheckBox();

    CheckBox *checkBox;

    void render(QPainter *painter);

    void processMouseEvent(QMouseEvent *e);

    bool mouseActive;

    IconButton *okB, *cancelB;

    virtual void resetDialogState();

    void resize(QSize size);

    QSize mainWidgetSize;
};

#endif //OPTIONSSELECTOR_H
