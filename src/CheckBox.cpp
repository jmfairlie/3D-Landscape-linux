#include "CheckBox.h"

CheckBox::CheckBox(int x, int y, int width, int height,
                  int layout, QString title)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->layout = layout;

    this->spacing = 5;

    this->title = title;

    this->title_size = (int)(this->height * 0.2);

    this->titleTextR.setX(this->x + this->spacing);
    this->titleTextR.setY(this->y + this->spacing);
    this->titleTextR.setWidth(this->width - this->spacing*2);
    this->titleTextR.setHeight(this->title_size - this->spacing*2);
}

//items QList should be populated beforehand
void CheckBox::initialize()
{
    int currX = this->x + this->spacing;
    int currY = this->y + this->spacing + this->title_size;

    int itemHeight = 0;
    int itemWidth = 0;
    int itemSpacing = 0;

    if(!layout)
    {
        //itemHeight = (this->height - this->title_size - this->spacing -
        //        (this->items.size() * this->spacing)) / this->items.size();

        itemHeight = (this->height - this->title_size - this->spacing -
                (this->items.size() * this->spacing)) / this->items.size();

        //currX += (this->width/2) - (itemSize/2);
        if(this->items.size() > 1)
            itemSpacing = (this->height - this->title_size - this->spacing*2 -
                                                (itemHeight * this->items.size())) / (this->items.size() - 1);
        else
            itemSpacing = this->spacing;
    }
    else
    {
        //itemWidth = (this->width - this->spacing -
        //        (this->items.size() * this->spacing)) / this->items.size();

        itemWidth = (this->width - this->spacing -
                (this->items.size() * this->spacing)) / this->items.size();

        if(this->items.size() > 1)
            itemSpacing = (this->width - this->spacing*2 -
                                                (itemWidth * this->items.size())) / (this->items.size() - 1);
        else
            itemSpacing = this->spacing;

        //currY += (this->height/2) - (itemSize/2) + this->title_size;
    }

    for(int i = 0; i < this->items.size(); i++)
    {
        if(layout)
        {
            this->items[i]->width = itemWidth;
            this->items[i]->height = this->height - this->spacing*2;
            this->items[i]->x = currX;
            this->items[i]->y = currY;
            currX += this->items[i]->width + itemSpacing;
        }
        else
        {
            this->items[i]->height = itemHeight;
            this->items[i]->width = this->width - this->spacing*2;
            this->items[i]->x = currX;
            this->items[i]->y = currY;
            currY += this->items[i]->height + itemSpacing;
        }

        this->items[i]->calculateInnerPositions();
    }
}

void CheckBox::render(QPainter *painter)
{
    QBrush brush(QColor(255, 255, 255, 200));
    painter->setBrush(brush);

    painter->setPen(QPen(QColor(0, 0, 0, 200)));

    painter->drawRoundedRect(this->x, this->y, this->width, this->height, 5, 5);


    QFont titleFont("Arial");
    titleFont.setPixelSize(this->titleTextR.height());

    QFontMetrics fm(titleFont);

    int textWidthInPixels = fm.width(this->title);

    if ( textWidthInPixels > (this->titleTextR.width()))
    {
         titleFont.setStretch
                 ((100 * this->titleTextR.width()) / textWidthInPixels) ;
    }


    painter->setFont(titleFont);

    painter->drawText(this->titleTextR, Qt::AlignCenter, this->title);

    for(int i = 0; i < this->items.size(); i++)
    {
        this->items[i]->render(painter);
    }
}

void CheckBox::resize(int x, int y, int width, int height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;

    this->title_size = (int)(this->height * 0.2);

    this->titleTextR.setX(this->x + this->spacing);
    this->titleTextR.setY(this->y + this->spacing);
    this->titleTextR.setWidth(this->width - this->spacing*2);
    this->titleTextR.setHeight(this->title_size - this->spacing*2);

    this->initialize();
}

void CheckBox::processMouseEvent(QMouseEvent *e)
{
    for(int i = 0; i < this->items.size(); i++)
    {
        if(this->items[i]->isInside(e->x(), e->y()))
        {
            cout << "Item clicked: " <<
                    this->items[i]->label.toStdString() << endl;
        }
    }
}

CheckBox::~CheckBox()
{
    for(int i = 0; i < this->items.size(); i++)
    {
        delete this->items[i];
    }
}


