#include "FilterItem.h"

FilterItem::FilterItem(QString label, int x, int y, int w, int h)
{
    this->label = label;
    this->x = x;
    this->y = y;
    this->width = w;
    this->height = h;

    this->spacing = 2;
    
    this->wasMouseHover = false;

    if(w && h)
        this->calculateInnerPositions();
}

//if inside container, should be called after adding to this container
//inside container's initialize function
void FilterItem::calculateInnerPositions()
{
    //rectangle to draw an ellipse
    this->h_circle = this->height - this->spacing*2;
    this->w_circle = this->width - this->spacing*2;
    this->y_circle = this->y + this->spacing;
    this->x_circle = this->x + this->spacing;

    //inner rectangle for text
    this->height_text = this->h_circle / 4;
    this->width_text = this->w_circle - this->spacing * 2;
    this->y_text = this->y_circle + (this->h_circle/2) - (this->height_text/2);
    this->x_text = this->x_circle + (this->w_circle/2) - (this->width_text/2);

    //cross rectangle    
    this->width_rect = (int)(this->width * 0.1);
    this->height_rect = this->width_rect;
    this->x_rect = this->x + this->width - this->width_rect;
    this->y_rect = this->y;

    //text font
    this->setTextFont();
}

void FilterItem::setTextFont()
{
    this->textFont.setFamily("Arial");
    this->textFont.setPixelSize(this->height_text / 2);
    this->textFont.setStretch(100);

    QFontMetrics fm(this->textFont);

    int textWidthInPixels = fm.width(this->label);

    if (textWidthInPixels > (this->width_text))
    {
         this->textFont.setStretch
                 ((100 * this->width_text) / textWidthInPixels) ;
    }
}

bool FilterItem::crossClicked(int x, int y)
{
    if(x >= this->x_rect &&
       x <= (this->x_rect + this->width_rect) &&
       y >= this->y_rect &&
       y <= (this->y_rect + this->height_rect))
    {
        return true;
    }
    else
        return false;
}

bool FilterItem::isInside(int x, int y)
{
    if(x >= this->x_circle &&
       x <= (this->x_circle + this->w_circle) &&
       y >= this->y_circle &&
       y <= (this->y_circle + this->h_circle))
    {
        return true;
    }
    else
        return false;
}

void FilterItem::render(QPainter *painter)
{
    //main rectangle
    QBrush brush(QColor(255, 255, 255, 200));
    QPen pen(QColor(0, 0, 0, 255));

    painter->setBrush(brush);

    painter->setFont(this->textFont);

    //painter->setFont(QFont("Arial", this->height / (this->buttonLabel.length())));
    //painter->drawRoundedRect(this->x, this->y, this->width, this->height, 5, 5);

    //ellipse
    painter->setBrush(QBrush(QColor(255,0,0,200)));
    painter->drawEllipse(this->x_circle, this->y_circle, this->w_circle, this->h_circle);

    pen.setWidth(2);
    painter->setPen(pen);
    painter->drawLine(this->x_rect, this->y_rect, this->x_rect + this->width_rect,
                      this->y_rect + this->height_rect);

    painter->drawLine(this->x_rect, this->y_rect + this->width_rect,
                      this->x_rect + this->width_rect, this->y_rect);


    pen.setColor(QColor(255,255,255,200));
    pen.setWidth(1);
    painter->setPen(pen);
    painter->drawText(this->x_text, this->y_text, this->width_text, this->height_text,
                      Qt::AlignCenter, this->label);

    painter->setPen(QPen());

}
