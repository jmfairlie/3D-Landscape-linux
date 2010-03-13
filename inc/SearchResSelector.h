#ifndef SRSELECTOR_H
#define SRSELECTOR_H

//#include "Button2D.h"
#include "IconButton.h"
#include "Dialog2D.h"

#include <QPainter>
#include <QMouseEvent>
#include <cmath>

class SearchResSelector : public Dialog2D
{
public:
    SearchResSelector(QSize mainWidgetSize);

    virtual ~SearchResSelector();

    void render(QPainter *painter);

    void processMouseEvent(QMouseEvent *e);

    void resize(QSize mainWidgetSize);

    bool mouseActive;

    int resultsNumber;

    IconButton *bramble, *bsearch;

    int spacing;

    virtual void resetDialogState();

    int r_x, r_y, r_width, r_height;
    int text_x, text_y, text_width, text_height;

    QSize mainWidgetSize;

    QFont resFont;

    void setResString(QString resStr);

private:
    QString resString;
    void setFont();
};


#endif
