#ifndef FILTERSELECTOR_H
#define FILTERSELECTOR_H

#include "FilterBox.h"
//#include "Button2D.h"
#include "IconButton.h"
#include "Dialog2D.h"

class FilterSelector : public Dialog2D
{    
public:
    FilterSelector(QSize mainWidgetSize);

    virtual ~FilterSelector();

    void createFilterBox();

    FilterBox *filterBox;

    void render(QPainter *painter);

    void processMouseEvent(QMouseEvent *e);
    bool mouseActive;

    IconButton *searchB, *cancelB;

    void resize(QSize size);

    virtual void resetDialogState();

    void appendFilter(QString filterName);

    QString clickedCircleName;

    QSize mainWidgetSize;
};

#endif //FILTERSELECTOR_H
