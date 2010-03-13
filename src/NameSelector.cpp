#include "NameSelector.h"

NameSelector::NameSelector(QSize mainWidgetSize)
{
    this->mainWidgetSize = mainWidgetSize;

    int height = (int)(this->mainWidgetSize.height() * 0.3);
    int width = (int)(this->mainWidgetSize.width() * 0.4);
    int x = this->mainWidgetSize.width() / 2 - width / 2;
    int y = this->mainWidgetSize.height() / 2 - height / 2;

    this->editBox = new EditBox(x, y, width, height, 0, "Enter name");
    this->createEditBox();

    this->mouseActive = false;

    height = (int)(this->mainWidgetSize.height() * 0.2);
    x = this->mainWidgetSize.width() / 2 - width / 2;
    y = this->mainWidgetSize.height() - height;
    width = height;

    //this->okB = new Button2D("OK", 0, y, width, height, true);
    this->okB = new IconButton("../res/images/png/button_ok.png", "OK", 0, y, width, height);

    x = this->mainWidgetSize.width() - width;
    //this->cancelB = new Button2D("Cancel", x, y, width, height, true);
    this->cancelB = new IconButton("../res/images/png/button_cancel.png", "Cancel", x, y, width, height);
}

void NameSelector::createEditBox()
{
    EditItem *item = new EditItem("Destination name:");

    this->editBox->items.append(item);

    this->editBox->initialize();
}

void NameSelector::resetDialogState()
{
    this->dialogResult = -1;
    this->okB->selected = false;
    this->cancelB->selected = false;
}

void NameSelector::resize(QSize size)
{
    this->mainWidgetSize = size;

    int height = (int)(this->mainWidgetSize.height() * 0.3);
    int width = (int)(this->mainWidgetSize.width() * 0.4);
    int x = this->mainWidgetSize.width() / 2 - width / 2;
    int y = this->mainWidgetSize.height() / 2 - height / 2;

    this->editBox->resize(x, y, width, height);

    height = (int)(this->mainWidgetSize.height() * 0.2);
    x = this->mainWidgetSize.width() / 2 - width / 2;
    y = this->mainWidgetSize.height() - height;
    width = height;

    this->okB->resize(0, y, width, height);

    x = this->mainWidgetSize.width() - width;
    this->cancelB->resize(x, y, width, height);
}

void NameSelector::render(QPainter *painter)
{
    this->editBox->render(painter);
    this->okB->render(painter);
    this->cancelB->render(painter);
}

void NameSelector::processKeyEvent(QKeyEvent *e)
{
    this->editBox->processKeyEvent(e);
}

void NameSelector::processMouseEvent(QMouseEvent *e)
{
    if (e->type() == QEvent::MouseButtonPress)
    {
        if(this->okB->isInside(e->x(), e->y()))
        {
            this->dialogResult = 0;
        }

        if(this->cancelB->isInside(e->x(), e->y()))
        {
            this->dialogResult = 1;
        }

        this->editBox->processMouseEvent(e);
        this->mouseActive = true;
    }
    else if(e->type() == QEvent::MouseMove)
    {
    }
    else if(e->type() == QEvent::MouseButtonRelease)
    {
        this->mouseActive = false;
    }
}

NameSelector::~NameSelector()
{
    delete this->editBox;
}

