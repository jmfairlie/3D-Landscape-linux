#include "DistanceSelector.h"

DistanceSelector::DistanceSelector(QSize mainWidgetSize)
{
    this->mainWidgetSize = mainWidgetSize;

    int height = (int)(this->mainWidgetSize.height() * 0.2);
    int width = (int)(this->mainWidgetSize.width() * 0.3);
    int x = (int)(this->mainWidgetSize.width() / 2 - width / 2);
    int y = (int)(this->mainWidgetSize.height() - height);

    this->radioButton = new RadioButton2D(x, y, width, height, 1);

    this->createRadioButton();

    this->circleCentreX = 0;
    this->circleCentreY = 0;
    this->circleEdgeX = 0;
    this->circleEdgeY = 0;

    this->radius = 0;

    this->mouseActive = false;

    width = height;
    //this->bok = new Button2D("OK", 0, y, width, height, true);
    this->bok = new IconButton("../res/images/png/button_ok.png", "OK", 0, y, width, height);

    x = this->mainWidgetSize.width() - width;
    //this->bcancel = new Button2D("Cancel", x, y, width, height, true);
    this->bcancel = new IconButton("../res/images/png/button_cancel.png", "Cancel", x, y, width, height);
}

void DistanceSelector::resetDialogState()
{
    this->dialogResult = -1;
    this->bok->selected = false;
    this->bcancel->selected = false;
}

void DistanceSelector::resize(QSize mainWidgetSize)
{
    this->mainWidgetSize = mainWidgetSize;

    int height = (int)(this->mainWidgetSize.height() * 0.2);
    int width = (int)(this->mainWidgetSize.width() * 0.3);
    int x = (int)(this->mainWidgetSize.width() / 2 - width / 2);
    int y = (int)(this->mainWidgetSize.height() - height);

    this->radioButton->resize(x, y, width, height);

    width = height;

    this->bok->resize(0, y, width, height);

    x = this->mainWidgetSize.width() - width;
    this->bcancel->resize(x, y, width, height);
}

void DistanceSelector::createRadioButton()
{
    Button2D *button = new Button2D("Bicycle");
    Button2D *button2 = new Button2D("Walk");
    Button2D *button3 = new Button2D("Car");

    this->radioButton->buttons.append(button);
    this->radioButton->buttons.append(button2);
    this->radioButton->buttons.append(button3);

    this->radioButton->initialize();
}

void DistanceSelector::render(QPainter *painter)
{

    QBrush brush(QColor(255,255,0,190));
    QPen pen(QColor(255,255,0,190));
    pen.setWidth(3);

    painter->setBrush(brush);
    painter->setPen(pen);


    this->radius = (int)sqrt((this->circleEdgeX - this->circleCentreX) *
                 (this->circleEdgeX - this->circleCentreX) +
                 (this->circleEdgeY - this->circleCentreY) *
                 (this->circleEdgeY - this->circleCentreY));

    if((this->circleEdgeX != this->circleCentreX) &&
       (this->circleEdgeY != this->circleCentreY))//this->mouseActive)
    {

        painter->drawEllipse(this->circleCentreX - 5,
                             this->circleCentreY - 5, 10, 10);
        painter->drawLine(this->circleCentreX, this->circleCentreY,
                          this->circleEdgeX, this->circleEdgeY);

        painter->setBrush(QBrush(Qt::NoBrush));

        painter->drawEllipse(this->circleCentreX - radius,
                             this->circleCentreY - radius, radius*2, radius*2);

        painter->setPen(QPen());
    }

    this->radioButton->render(painter);
    this->bok->render(painter);
    this->bcancel->render(painter);
}

void DistanceSelector::processMouseEvent(QMouseEvent *e)
{
    if (e->type() == QEvent::MouseButtonPress)
    {
        bool buttonPressed = false;

        if(this->bok->isInside(e->x(), e->y()))
        {
            this->dialogResult = 0;
            buttonPressed = true;
        }
        if(this->bcancel->isInside(e->x(), e->y()))
        {
            this->dialogResult = 1;
            buttonPressed = true;
        }

        //only if buttons were not pressed
        if(!buttonPressed)
        {
            this->radioButton->processMouseEvent(e);

            this->circleCentreX = e->x();
            this->circleCentreY = e->y();
            this->circleEdgeX = e->x();
            this->circleEdgeY = e->y();

            this->mouseActive = true;
        }
    }
    else if(e->type() == QEvent::MouseMove)
    {
        if(this->mouseActive)
        {
            this->circleEdgeX = e->x();
            this->circleEdgeY = e->y();
        }
    }
    else if(e->type() == QEvent::MouseButtonRelease)
    {
        this->mouseActive = false;
    }
}

DistanceSelector::~DistanceSelector()
{
    delete this->radioButton;
    delete this->bok;
    delete this->bcancel;
}
