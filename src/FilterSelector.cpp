#include "FilterSelector.h"

FilterSelector::FilterSelector(QSize mainWidgetSize)
{
    this->mainWidgetSize = mainWidgetSize;

    int height = (int)(this->mainWidgetSize.height() * 0.2);
    int width = (int)(this->mainWidgetSize.width() * 0.5);
    int x = this->mainWidgetSize.width() / 2 - width / 2;
    int y = this->mainWidgetSize.height() - height;

    this->filterBox = new FilterBox(x, y, width, height, 1, "Filters");
    this->createFilterBox();

    this->mouseActive = false;

    this->clickedCircleName = "";

    //height = (int)(this->mainWidgetSize.height() * 0.2);
    x = 0;
    y = 0;
    width = height;

    //this->searchB = new Button2D("Search", x, y, width, height, true);
    this->searchB = new IconButton("../res/images/png/button_search.png", "Search", x, y, width, height);
    x = this->mainWidgetSize.width() - width;
    y = 0;
    //this->cancelB = new Button2D("Cancel", x, y, width, height, true);
    this->cancelB = new IconButton("../res/images/png/button_cancel.png", "Cancel", x, y, width, height);
}

void FilterSelector::appendFilter(QString filterName)
{
    //=true, if filter with such name already exists
    bool hasFilter = false;

    for(int i = 0; i < this->filterBox->items.size(); i++)
    {
        if(this->filterBox->items[i]->label == filterName)
        {
            hasFilter = true;
            break;
        }
    }

    if(!hasFilter)
    {
        this->filterBox->items.append(
                       new FilterItem(filterName));
        this->filterBox->initialize();
    }
}

void FilterSelector::resetDialogState()
{
    this->dialogResult = -1;
    this->searchB->selected = false;
    this->cancelB->selected = false;
}

void FilterSelector::createFilterBox()
{
    /*FilterItem *item = new FilterItem("Destination name");
    FilterItem *item2 = new FilterItem("Fees");
    FilterItem *item3 = new FilterItem("Distance");
    FilterItem *item4 = new FilterItem("Services");

    this->filterBox->items.append(item);
    this->filterBox->items.append(item2);
    this->filterBox->items.append(item3);
    this->filterBox->items.append(item4);*/

    this->filterBox->initialize();
}

void FilterSelector::resize(QSize size)
{
    this->mainWidgetSize = size;

    int height = (int)(this->mainWidgetSize.height() * 0.2);
    int width = (int)(this->mainWidgetSize.width() * 0.5);
    int x = this->mainWidgetSize.width() / 2 - width / 2;
    int y = this->mainWidgetSize.height() - height;

    this->filterBox->resize(x, y, width, height);

    //height = (int)(this->mainWidgetSize.height() * 0.2);
    x = 0;
    y = 0;
    width = height;

    this->searchB->resize(x, y, width, height);
    x = this->mainWidgetSize.width() - width;
    y = 0;
    this->cancelB->resize(x, y, width, height);
}

void FilterSelector::render(QPainter *painter)
{
    this->filterBox->render(painter);
    this->searchB->render(painter);
    this->cancelB->render(painter);
}

void FilterSelector::processMouseEvent(QMouseEvent *e)
{
    if (e->type() == QEvent::MouseButtonPress)
    {
        if(this->searchB->isInside(e->x(), e->y()))
        {
            this->dialogResult = 0;
        }

        if(this->cancelB->isInside(e->x(), e->y()))
        {
            this->dialogResult = 1;
        }

        //returns the name of the clicked circle, if any or
        //empty string
        this->clickedCircleName = this->filterBox->processMouseEvent(e);

        if(this->clickedCircleName != "")
        {
            //go to filter dialog
            this->dialogResult = 2;             
            return;
        }

        this->mouseActive = true;
    }
    else if(e->type() == QEvent::MouseMove)
    {
        this->filterBox->processMouseEvent(e);
    }
    else if(e->type() == QEvent::MouseButtonRelease)
    {
        this->mouseActive = false;
    }
}

FilterSelector::~FilterSelector()
{
    delete this->filterBox;
}

