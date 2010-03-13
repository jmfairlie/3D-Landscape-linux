#include "FilterBox.h"

FilterBox::FilterBox(){}

FilterBox::FilterBox(int x, int y, int width, int height,
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

void FilterBox::resize(int x, int y, int width, int height)
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

//items QList should be populated beforehand
void FilterBox::initialize()
{
    if(!this->items.size())
        return;

    int currX = this->x + this->spacing;
    int currY = this->y + this->spacing + this->title_size;

    int itemSize = 0;

    //distance between items
    int itemSpacing = 0;

    if(!layout)
    {
        itemSize = min(((this->height - this->title_size - this->spacing -
                (this->items.size() * this->spacing)) / this->items.size()), this->width - this->spacing * 2);

        currX += ((this->width - this->spacing * 2)/2) - (itemSize/2);

        if(this->items.size() > 1)
            itemSpacing = (this->height - this->title_size - this->spacing*2 -
                                                (itemSize * this->items.size())) / (this->items.size() - 1);
        else
            itemSpacing = this->spacing;
    }
    else
    {
        itemSize = min(((this->width - this->spacing -
                (this->items.size() * this->spacing)) / this->items.size()),
                this->height - this->title_size - this->spacing * 2);

        currY += ((this->height - this->title_size - this->spacing * 2)/2) - (itemSize/2);

        if(this->items.size() > 1)
            itemSpacing = (this->width - this->spacing*2 -
                                                (itemSize * this->items.size())) / (this->items.size() - 1);
        else
            itemSpacing = this->spacing;
    }


    for(int i = 0; i < this->items.size(); i++)
    {
        if(layout)
        {
            this->items[i]->width = itemSize;
            this->items[i]->height = this->items[i]->width;
            this->items[i]->x = currX;
            this->items[i]->y = currY;
            currX += this->items[i]->width + itemSpacing;
        }
        else
        {
            this->items[i]->height = itemSize;
            this->items[i]->width = this->items[i]->height;
            this->items[i]->x = currX;
            this->items[i]->y = currY;
            currY += this->items[i]->height + itemSpacing;
        }

        this->items[i]->calculateInnerPositions();
    }
}

void FilterBox::render(QPainter *painter)
{
    QBrush brush(QColor(255, 255, 255, 200));
    painter->setBrush(brush);

    painter->setPen(QPen(QColor(255, 255, 255, 200)));

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
    painter->setPen(QPen(QColor(0, 0, 0, 200)));

    painter->drawText(this->titleTextR, Qt::AlignCenter, this->title);

    for(int i = 0; i < this->items.size(); i++)
    {
        this->items[i]->render(painter);
    }
}

QString FilterBox::processMouseEvent(QMouseEvent *e)
{
    if(e->type() == QEvent::MouseButtonPress)
    {
        for(int i = 0; i < this->items.size(); i++)
        {
            if(this->items[i]->crossClicked(e->x(), e->y()))
            {
                cout << "CROSS CLICKED!!!" << endl;
                delete this->items[i];
                this->items.removeAt(i);
                break;
            }
            if(this->items[i]->isInside(e->x(), e->y()))
            {
                return this->items[i]->label;
            }
        }
    }
    else if(e->type() == QEvent::MouseMove)
    {
        for(int i = 0; i < this->items.size(); i++)
        {
            if(this->items[i]->isInside(e->x(), e->y()))
            {                
                if(!this->items[i]->wasMouseHover)
                {
                    this->items[i]->y -= 20;
                    this->items[i]->calculateInnerPositions();
                    this->items[i]->wasMouseHover = true;
                }
            }
            else
            {
                //return to previous state
               if(this->items[i]->wasMouseHover)
               {
                   this->items[i]->y += 20;
                   this->items[i]->calculateInnerPositions();
                   this->items[i]->wasMouseHover = false;
               }
            }
        }
    }

    return "";
}

FilterBox::~FilterBox()
{
    for(int i = 0; i < this->items.size(); i++)
    {
        delete this->items[i];
    }
}


