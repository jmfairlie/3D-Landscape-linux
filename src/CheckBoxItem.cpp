#include "CheckBoxItem.h"

CheckBoxItem::CheckBoxItem(QString label, int x, int y, int w, int h)
{
    this->label = label;
    this->x = x;
    this->y = y;
    this->width = w;
    this->height = h;

    this->checked = false;

    this->spacing = 2;

    if(w && h)
        this->calculateInnerPositions();
}

bool CheckBoxItem::isInside(int x, int y)
{
    if(x >= this->x && x <= (this->x + this->width) &&
       y >= this->y && y <= (this->y + this->height))
    {
        this->checked = !this->checked;
        return true;
    }
    else
        return false;
}

//if inside container, should be called after adding to this container
//inside container's initialize function
void CheckBoxItem::calculateInnerPositions()
{
    this->h_inner = (int)(this->height*0.5);
    this->w_inner = (int)(this->h_inner);
    this->x_inner = (int)(this->x + this->spacing);
    this->y_inner = (int)(this->y + (this->height/2) -
                    (this->h_inner/2));

    this->y_text = this->y_inner;
    this->x_text = this->x_inner + this->w_inner + this->spacing;

    this->height_text = this->h_inner;
    this->width_text = this->width - this->w_inner - this->spacing*3;

    this->itemFont.setFamily("Arial");
    this->itemFont.setPixelSize(this->height_text);
    this->itemFont.setStretch(100);

    QFontMetrics fm(this->itemFont);

    int textWidthInPixels = fm.width(this->width_text);

    if (textWidthInPixels > (this->width_text))
    {
         this->itemFont.setStretch
                 ((100 * this->width_text) / textWidthInPixels) ;
    }
}

void CheckBoxItem::render(QPainter *painter)
{
    //main rectangle
    QBrush brush(QColor(255, 255, 255, 200));
    QPen pen(QColor(0, 0, 0, 255));

    painter->setBrush(brush);
    painter->setPen(pen);
    painter->setFont(this->itemFont);

    //painter->setFont(QFont("Arial", this->height / (this->buttonLabel.length())));

    painter->drawRoundedRect(this->x, this->y, this->width, this->height, 5, 5);

    painter->setBrush(QBrush(Qt::NoBrush));

    painter->drawRect(this->x_inner, this->y_inner, this->w_inner, this->h_inner);

    painter->drawText(this->x_text, this->y_text, this->width_text, this->height_text,
                      Qt::AlignCenter, this->label);

    if(this->checked)
    {
        painter->setBrush(QBrush(QColor(0,255,0,255)));

        painter->drawLine(x_inner, y_inner, x_inner + w_inner,
                          y_inner + h_inner);

        painter->drawLine(x_inner, y_inner + h_inner, x_inner + w_inner,
                          y_inner);
    }

}
