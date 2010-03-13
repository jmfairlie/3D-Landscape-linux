#include "ResultItem.h"

ResultItem::ResultItem(int width, int height, QString label)
{
    this->width = width;
    this->height = height;
    this->label = label;
}

void ResultItem::render(QPainter *painter, int x, int y)
{
    painter->setBrush(QBrush(QColor(255,255,0,190)));
    painter->setPen(QPen(QColor(0,0,0,255)));

    this->x = x;
    this->y = y;

    painter->drawEllipse(this->x - this->width/2,
         this->y - this->height/2, this->width, this->height);

    if(this->label != "")
    {
        int text_x = this->x - this->width*2;
        int text_y = this->y - this->height/2 - this->height/2;
        int text_w = this->width*4;
        int text_h = this->height/2;

        QFont titleFont;
        titleFont.setFamily("Arial");
        titleFont.setBold(true);
        titleFont.setPixelSize(text_h);

        painter->setPen(QPen(QColor(255,255,255,255)));
        painter->setFont(titleFont);
        painter->drawText(text_x, text_y, text_w, text_h,
                          Qt::AlignCenter, this->label);
    }
}

bool ResultItem::isInside(int x, int y)
{
    int centre_x = this->x;
    int centre_y = this->y;
    int radius = this->width / 2;

    if(((x - centre_x)*(x - centre_x) +
       (y - centre_y)*(y - centre_y)) < radius * radius)
    {
        return true;
    }
    else
    {
        return false;
    }
}
