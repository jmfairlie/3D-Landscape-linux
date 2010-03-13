#include "PricesSelector.h"

PricesSelector::PricesSelector(QSize mainWidgetSize)
{
    this->mainWidgetSize = mainWidgetSize;

    int height = (int)(this->mainWidgetSize.height() * 0.7);
    int width = (int)(this->mainWidgetSize.width() * 0.4);
    int x = this->mainWidgetSize.width() / 2 - width / 2;
    int y = this->mainWidgetSize.height() / 2 - height / 2;

    this->sliderBox = new SliderBox(x, y, width, height, 0, "Make your choice!");
    this->createSliderBox();

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

void PricesSelector::createSliderBox()
{
    Slider2D *item = new Slider2D("Entrance", 0, 255);
    Slider2D *item2 = new Slider2D("Wardrobe", 0, 255);
    Slider2D *item3 = new Slider2D("Bathroom", 0, 255);

    this->sliderBox->items.append(item);
    this->sliderBox->items.append(item2);
    this->sliderBox->items.append(item3);

    this->sliderBox->initialize();
}

void PricesSelector::resetDialogState()
{
    this->dialogResult = -1;
    this->okB->selected = false;
    this->cancelB->selected = false;
}

void PricesSelector::resize(QSize size)
{
    this->mainWidgetSize = size;

    int height = (int)(this->mainWidgetSize.height() * 0.7);
    int width = (int)(this->mainWidgetSize.width() * 0.4);
    int x = this->mainWidgetSize.width() / 2 - width / 2;
    int y = this->mainWidgetSize.height() / 2 - height / 2;

    this->sliderBox->resize(x, y, width, height);

    height = (int)(this->mainWidgetSize.height() * 0.2);
    x = this->mainWidgetSize.width() / 2 - width / 2;
    y = this->mainWidgetSize.height() - height;
    width = height;

    this->okB->resize(0, y, width, height);

    x = this->mainWidgetSize.width() - width;
    this->cancelB->resize(x, y, width, height);
}

void PricesSelector::render(QPainter *painter)
{
    this->sliderBox->render(painter);
    this->okB->render(painter);
    this->cancelB->render(painter);
}

void PricesSelector::processMouseEvent(QMouseEvent *e)
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

        this->sliderBox->processMouseEvent(e);
        this->mouseActive = true;
    }
    else if(e->type() == QEvent::MouseMove)
    {
        if(this->mouseActive)
            this->sliderBox->processMouseEvent(e);
    }
    else if(e->type() == QEvent::MouseButtonRelease)
    {
        this->mouseActive = false;
    }
}

PricesSelector::~PricesSelector()
{
    delete this->sliderBox;
}

