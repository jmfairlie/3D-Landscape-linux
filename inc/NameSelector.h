#ifndef NAMESELECTOR_H
#define NAMESELECTOR_H

#include "EditBox.h"
//#include "Button2D.h"
#include "IconButton.h"
#include "Dialog2D.h"

class NameSelector : public Dialog2D
{
public:
    NameSelector(QSize mainWidgetSize);

    virtual ~NameSelector();

    void createEditBox();

    EditBox *editBox;

    void render(QPainter *painter);

    virtual void resetDialogState();
    void processMouseEvent(QMouseEvent *e);
    void processKeyEvent(QKeyEvent *e);

    bool mouseActive;

    IconButton *okB, *cancelB;

    void resize(QSize size);
    QSize mainWidgetSize;
};

#endif //NAMESELECTOR_H
