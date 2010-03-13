#include "RadioButton2D.h"

RadioButton2D::RadioButton2D(){}

RadioButton2D::RadioButton2D(int x, int y, int width, int height,
                  int layout)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->layout = layout;

    this->spacing = 5;
}

void RadioButton2D::resize(int x, int y, int width, int height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->initialize();
}

//buttons QList should be populated beforehand
void RadioButton2D::initialize()
{   
    int currX = this->x + this->spacing;
    int currY = this->y + this->spacing;

    int buttonSize = 0;

    //distance between buttons
    int buttonSpacing = 0;

    if(!layout)
    {
        buttonSize = min(((this->height - this->spacing -
                (this->buttons.size() * this->spacing)) / this->buttons.size()), this->width - this->spacing * 2);

        currX += ((this->width - this->spacing * 2)/2) - (buttonSize/2);

        if(this->buttons.size() > 1)
            buttonSpacing = (this->height - this->spacing*2 -
                                                (buttonSize * this->buttons.size())) / (this->buttons.size() - 1);
        else
            buttonSpacing = this->spacing;
    }
    else
    {
        buttonSize = min(((this->width - this->spacing -
                (this->buttons.size() * this->spacing)) / this->buttons.size()), this->height - this->spacing * 2);

        currY += ((this->height - this->spacing * 2)/2) - (buttonSize/2);

        if(this->buttons.size() > 1)
            buttonSpacing = (this->width - this->spacing*2 -
                                                (buttonSize * this->buttons.size())) / (this->buttons.size() - 1);
        else
            buttonSpacing = this->spacing;
    }


    for(int i = 0; i < this->buttons.size(); i++)
    {
        if(layout)
        {
            this->buttons[i]->width = buttonSize;
            this->buttons[i]->height = this->buttons[i]->width;
            this->buttons[i]->x = currX;
            this->buttons[i]->y = currY;
            currX += this->buttons[i]->width + buttonSpacing;
        }
        else
        {
            this->buttons[i]->height = buttonSize;
            this->buttons[i]->width = this->buttons[i]->height;
            this->buttons[i]->x = currX;
            this->buttons[i]->y = currY;
            currY += this->buttons[i]->height + buttonSpacing;
        }

        this->buttons[i]->calculateInnerPositions();
    }
}

void RadioButton2D::render(QPainter *painter)
{
    QBrush brush(QColor(255, 255, 255, 200));
    painter->setBrush(brush);

    painter->setPen(QPen(QColor(255, 255, 255, 200)));

    painter->drawRoundedRect(this->x, this->y, this->width, this->height, 5, 5);

    for(int i = 0; i < this->buttons.size(); i++)
    {
        this->buttons[i]->render(painter);
    }
}

bool RadioButton2D::isInside(int x, int y)
{
    if((x > this->x) &&
       (x < (this->x + this->width)) &&
       (y > this->y) &&
       (y < (this->y + this->height)))
    {
        return true;
    }
    else
        return false;
}


void RadioButton2D::resetSelections()
{
    for(int i = 0; i < this->buttons.size(); i++)
        this->buttons[i]->selected = false;
}

void RadioButton2D::processMouseEvent(QMouseEvent *e)
{
    if(this->isInside(e->x(), e->y()))
    {
        this->resetSelections();

        for(int i = 0; i < this->buttons.size(); i++)
        {
            if(this->buttons[i]->isInside(e->x(), e->y()))
            {
                cout << "Button clicked: " <<
                        this->buttons[i]->buttonLabel.toStdString() << endl;
            }
        }
    }
}

RadioButton2D::~RadioButton2D()
{
    for(int i = 0; i < this->buttons.size(); i++)
    {
        delete this->buttons[i];
    }
}

