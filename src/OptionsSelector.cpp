#include "OptionsSelector.h"

OptionsSelector::OptionsSelector(QSize mainWidgetSize)
{
    this->mainWidgetSize = mainWidgetSize;

    int height = (int)(this->mainWidgetSize.height() * 0.7);
    int width = (int)(this->mainWidgetSize.width() * 0.4);
    int x = (int)(this->mainWidgetSize.width() / 2 - width / 2);
    int y = (int)(this->mainWidgetSize.height() / 2 - height / 2);

    this->checkBox = new CheckBox(x, y, width, height, 0, "Make your choice!");
    this->createCheckBox();

    this->mouseActive = false;

    height = (int)(this->mainWidgetSize.height() * 0.2);
    x = (int)(this->mainWidgetSize.width() / 2 - width / 2);
    y = (int)(this->mainWidgetSize.height() - height);
    width = height;

    //this->okB = new Button2D("OK", 0, y, width, height, true);
    this->okB = new IconButton("../res/images/png/button_ok.png", "OK", 0, y, width, height);


    x = this->mainWidgetSize.width() - width;
    //this->cancelB = new Button2D("Cancel", x, y, width, height, true);
    this->cancelB = new IconButton("../res/images/png/button_cancel.png", "Cancel", x, y, width, height);
}

void OptionsSelector::resetDialogState()
{
    this->dialogResult = -1;
    this->okB->selected = false;
    this->cancelB->selected = false;
}

void OptionsSelector::createCheckBox()
{
    CheckBoxItem *item = new CheckBoxItem("Dance area");
    CheckBoxItem *item2 = new CheckBoxItem("Live music");
    CheckBoxItem *item3 = new CheckBoxItem("Kilpailu");
    CheckBoxItem *item4 = new CheckBoxItem("Karaoke");
    CheckBoxItem *item5 = new CheckBoxItem("Board/video games");
    CheckBoxItem *item6 = new CheckBoxItem("Smoking area");
    CheckBoxItem *item7 = new CheckBoxItem("Terrace");
    CheckBoxItem *item8 = new CheckBoxItem("Jukebox");
    CheckBoxItem *item9 = new CheckBoxItem("Internet");

    this->checkBox->items.append(item);
    this->checkBox->items.append(item2);
    this->checkBox->items.append(item3);
    this->checkBox->items.append(item4);
    this->checkBox->items.append(item5);
    this->checkBox->items.append(item6);
    this->checkBox->items.append(item7);
    this->checkBox->items.append(item8);
    this->checkBox->items.append(item9);

    this->checkBox->initialize();
}

void OptionsSelector::resize(QSize size)
{
    this->mainWidgetSize = size;

    int height = (int)(this->mainWidgetSize.height() * 0.7);
    int width = (int)(this->mainWidgetSize.width() * 0.4);
    int x = (int)(this->mainWidgetSize.width() / 2 - width / 2);
    int y = (int)(this->mainWidgetSize.height() / 2 - height / 2);

    this->checkBox->resize(x, y, width, height);

    height = (int)(this->mainWidgetSize.height() * 0.2);
    x = (int)(this->mainWidgetSize.width() / 2 - width / 2);
    y = (int)(this->mainWidgetSize.height() - height);
    width = height;

    this->okB->resize(0, y, width, height);

    x = this->mainWidgetSize.width() - width;
    this->cancelB->resize(x, y, width, height);
}

void OptionsSelector::render(QPainter *painter)
{
    this->checkBox->render(painter);
    this->okB->render(painter);
    this->cancelB->render(painter);
}

void OptionsSelector::processMouseEvent(QMouseEvent *e)
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

        this->checkBox->processMouseEvent(e);
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

OptionsSelector::~OptionsSelector()
{
    delete this->checkBox;
}

