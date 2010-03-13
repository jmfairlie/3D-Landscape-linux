#include "Button2D.h"

Button2D::Button2D(QString buttonLabel, int x, int y, int width, int height, bool background)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->buttonLabel = buttonLabel;
    this->selected = false;
    this->background = background;

    this->spacing = 2;

    if(width && height)
        this->calculateInnerPositions();
}


//works only for true circle
bool Button2D::isInside(int x, int y)
{
    int centre_x = this->bx + (this->bwidth / 2);
    int centre_y = this->by + (this->bheight / 2);
    int radius = this->bwidth / 2;

    if(((x - centre_x)*(x - centre_x) +
       (y - centre_y)*(y - centre_y)) < radius * radius)
    {
        this->selected = !this->selected;
        return true;
    }
    else
    {
        return false;
    }
}

void Button2D::resize(int x, int y, int width, int height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->calculateInnerPositions();
}

//inside container's initialize function
void Button2D::calculateInnerPositions()
{
    if(!background)
    {
        this->bx = x;
        this->by = y;
        this->bwidth = width;
        this->bheight = height;
    }
    else
    {
        this->bheight = (int)(this->height * 0.8);
        this->bwidth = (int)(this->width * 0.8);
        this->bx = (int)(this->x + (this->width/2) - (this->bwidth/2));
        this->by = (int)(this->y + (this->height/2) - (this->bheight/2));
    }

    this->height_text = this->bheight / 4;
    this->width_text = this->bwidth - this->spacing * 2;
    this->y_text = this->by + (this->bheight/2) - (this->height_text/2);
    this->x_text = this->bx + (this->bwidth/2) - (this->width_text/2);

    this->buttonFont.setFamily("Arial");
    this->buttonFont.setPixelSize(this->height_text);

    QFontMetrics fm(this->buttonFont);

    int textWidthInPixels = fm.width(this->width_text);
    this->buttonFont.setStretch(100);

    if (textWidthInPixels > (this->width_text))
    {
         this->buttonFont.setStretch
                 ((100 * this->width_text) / textWidthInPixels) ;
    }
}

void Button2D::render(QPainter *painter)
{
    if(background)
    {
        painter->setBrush(QBrush(QColor(255,255,255,200)));
        painter->drawRoundedRect(this->x, this->y, this->width, this->height, 5, 5);
    }

    int red = 255;
    int green = 0;
    int blue = 0;

    if(this->selected)
    {
        red = 0;
        green = 255;
    }

    QBrush brush(QColor(red, green, blue, 200));
    QPen pen(QColor(255, 255, 255, 255));

    painter->setBrush(brush);

    painter->setFont(this->buttonFont);

    painter->setPen(QPen());

    painter->drawEllipse(this->bx, this->by, this->bwidth, this->bheight);

    painter->setPen(pen);

    painter->drawText(this->x_text, this->y_text, this->width_text, this->height_text,
                      Qt::AlignCenter, this->buttonLabel);

    painter->setPen(QPen());

}
