#include "IconButton.h"


IconButton::IconButton(QString imagePath, QString buttonLabel, int x, int y,
             int width, int height)
{
    this->img = QImage(imagePath);

    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;

    this->buttonLabel = buttonLabel;
    this->selected = false;

    this->spacing = 2;

    if(width && height)
        this->calculateInnerPositions();
}

bool IconButton::isInside(int x, int y)
{
    if(x >= this->x && x <= (this->x + this->width) &&
       y >= this->y && y <= (this->y + this->height))
    {
        this->selected = !this->selected;
        return true;
    }
    else
        return false;
}

void IconButton::resize(int x, int y, int width, int height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->calculateInnerPositions();
}

//inside container's initialize function
void IconButton::calculateInnerPositions()
{    
    this->y_image = this->y;    
    this->height_image = this->height * 0.7;
    this->width_image = this->height_image;
    this->x_image = this->x + (this->width - this->width_image)/2;

    this->width_text = this->width - this->spacing*2;
    this->height_text = this->height*0.3 - this->spacing*2;
    this->x_text = this->x + this->spacing;
    this->y_text = this->y + this->spacing + this->height_image;

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

void IconButton::render(QPainter *painter)
{
    //image
    QRect drawRect(this->x_image, this->y_image, this->width_image, this->height_image);
    painter->drawImage(drawRect, this->img);

    
    QPen pen(QColor(255, 255, 255, 255));    

    painter->setFont(this->buttonFont);

    
    painter->setPen(pen);

    painter->drawText(this->x_text, this->y_text, this->width_text,
                      this->height_text,
                      Qt::AlignCenter, this->buttonLabel);

    painter->setPen(QPen());

}
