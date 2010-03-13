#include "Slider2D.h"

Slider2D::Slider2D(QString label, int rangeStart, int rangeEnd,
             int x, int y,
             int width, int height)
{
    this->label = label;
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;

    this->rangeStart = rangeStart;
    this->rangeEnd = rangeEnd;

    this->value.clear();
    this->value.sprintf("%d", this->rangeStart);

    this->spacing = 2;

    this->selected = false;

    if(width && height)
        this->calculateInnerPositions();
}

//inside container's initialize function
void Slider2D::calculateInnerPositions()
{
    this->title_size = (int)(this->height * 0.2);

    this->x_label = this->x + this->spacing;
    this->y_label = this->y + this->spacing;
    this->width_label = this->width - this->spacing*2;
    this->height_label = this->title_size - this->spacing*2;

    //label font
    this->titleFont.setFamily("Arial");
    this->titleFont.setPixelSize(this->height_label);
    this->titleFont.setStretch(100);

    QFontMetrics fm(this->titleFont);

    int textWidthInPixels = fm.width(this->label);

    if (textWidthInPixels > (this->width_label))
    {
         this->titleFont.setStretch
                 ((100 * this->width_label) / textWidthInPixels) ;
    }

    //value size
    this->width_value = (int)(this->width * 0.2 - this->spacing*2);

    cout << "Width value: " << this->width_value << endl;

    this->height_value = this->height - this->title_size - this->spacing*2;

    //slider line
    this->x1 = this->x + this->spacing;
    this->y1 = this->y + this->title_size +
               ((this->height - this->title_size)/2);

    this->x2 = this->x1 + (this->width - this->width_value - this->spacing*3);
    this->y2 = this->y1;

    //slider width and height
    this->sheight = (int)((this->height - this->title_size) * 0.6);
    this->swidth = (int)((this->width - this->spacing*2) * 0.02);
    this->sx = this->x1;
    this->sy = this->y + this->title_size +
               ((this->height - this->title_size)/2 - this->sheight/2);

    //value position
    this->x_value = this->x2 + this->spacing + this->swidth / 2;
    this->y_value = this->y + this->title_size + this->spacing;

    //value font
    this->valueFont.setFamily("Arial");
    this->valueFont.setPixelSize(this->height_value);
    this->valueFont.setStretch(100);

    QFontMetrics fm2(this->valueFont);

    QString tmp;
    tmp.sprintf("%d", this->rangeEnd);

    textWidthInPixels = fm2.width(tmp);

    if (textWidthInPixels > (this->width_value))
    {
         this->valueFont.setStretch
                 ((100 * this->width_value) / textWidthInPixels) ;
    }

    cout << "Font stretch: " << this->valueFont.stretch() << endl;

    //rectangle for mouse events
    int lineLength = (int)sqrt((this->x2 - this->x1)*(this->x2 - this->x1) +
                               (this->y2 - this->y1)*(this->y2 - this->y1));

    this->boundSliderR.setX(this->x1);
    this->boundSliderR.setY(this->sy);
    this->boundSliderR.setWidth(lineLength);
    this->boundSliderR.setHeight(this->sheight);
}

//if it is inside the rectangle, which bounds slider
bool Slider2D::isInside(int x, int y)
{
    if(x >= this->boundSliderR.x() &&
       x <= (this->boundSliderR.x() + this->boundSliderR.width()) &&
       y >= this->boundSliderR.y() &&
       y <= (this->boundSliderR.y() + this->boundSliderR.height()))
    {
        this->selected = !this->selected;
        this->updateValue(x);
        return true;
    }
    else
        return false;
}

void Slider2D::updateValue(int x)
{
    int lengthX = this->x2 - this->x1;
    int rangeLen = this->rangeEnd - this->rangeStart;

    float step = rangeLen / (float)lengthX;

    if((x <= this->x2) && (x >= this->x1))
    {
        this->value.clear();
        int newVal = (int)ceil((x - this->x1) * step);
        this->value.sprintf("%d", newVal);

        this->sx = x - this->swidth/2;
    }
}

void Slider2D::render(QPainter *painter)
{
    int red = 255;
    int green = 0;
    int blue = 0;

    QBrush brush(QColor(red, green, blue, 200));
    QPen pen(QColor(255, 255, 255, 255));

    painter->setBrush(QBrush(QColor(255,255,255,200)));

    //draw rectangle
    painter->drawRect(this->x, this->y, this->width, this->height);

    painter->setBrush(brush);

    painter->setFont(this->titleFont);

    //draw title
    painter->drawText(this->x_label, this->y_label, this->width_label,
                      this->height_label, Qt::AlignCenter, this->label);

    //draw slider line
    painter->drawLine(this->x1, this->y1, this->x2, this->y2);

    //draw slider
    painter->drawRect(this->sx, this->sy, this->swidth, this->sheight);

    painter->setFont(this->valueFont);

    //draw value
    painter->drawText(this->x_value, this->y_value, this->width_value,
                      this->height_value, Qt::AlignCenter, this->value);

    painter->setPen(QPen());
}
