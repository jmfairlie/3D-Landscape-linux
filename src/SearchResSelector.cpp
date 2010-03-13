#include "SearchResSelector.h"

SearchResSelector::SearchResSelector(QSize mainWidgetSize)
{
    this->mainWidgetSize = mainWidgetSize;

    this->spacing = 2;

    this->r_height = (int)(this->mainWidgetSize.height() * 0.2);
    this->r_width = (int)(this->mainWidgetSize.width() * 0.3);
    this->r_x = (int)(this->mainWidgetSize.width() / 2 - this->r_width / 2);
    this->r_y = (int)(this->mainWidgetSize.height() - this->r_height);

    this->text_height = this->r_height - this->spacing*2;
    this->text_width = this->r_width - this->spacing*2;
    this->text_x = this->r_x + this->spacing;
    this->text_y = this->r_y + this->spacing;
    
    this->mouseActive = false;

    int width = this->r_height;
    int height = this->r_height;
    //this->bsearch = new Button2D("Search", 0, this->r_y, width, height, true);
    this->bsearch = new IconButton("../res/images/png/button_search.png", "Search", 0, this->r_y, width, height);

    int x = this->mainWidgetSize.width() - width;
    //this->bramble = new Button2D("Ramble", x, this->r_y, width, height, true);
    this->bramble = new IconButton("../res/images/png/button_walk.png", "Ramble", x, this->r_y, width, height);

    this->resString = "Results: 0";
    this->setFont();
}

void SearchResSelector::resetDialogState()
{
    this->dialogResult = -1;
    this->bsearch->selected = false;
    this->bramble->selected = false;
}

void SearchResSelector::resize(QSize mainWidgetSize)
{
    this->mainWidgetSize = mainWidgetSize;

    this->r_height = (int)(this->mainWidgetSize.height() * 0.2);
    this->r_width = (int)(this->mainWidgetSize.width() * 0.3);
    this->r_x = (int)(this->mainWidgetSize.width() / 2 - this->r_width / 2);
    this->r_y = (int)(this->mainWidgetSize.height() - this->r_height);

    this->text_height = (int)(this->r_height * 0.5);
    this->text_width = (int)(this->r_width * 0.8);
    this->text_x = this->r_x + this->r_width / 2 - this->text_width / 2;
    this->text_y = this->r_y + this->r_height / 2 - this->text_height / 2;

    this->mouseActive = false;

    int width = this->r_height;
    int height = this->r_height;

    this->bsearch->resize(0, this->r_y, width, height);

    int x = this->mainWidgetSize.width() - width;
    this->bramble->resize(x, this->r_y, width, height);

    this->setFont();
}

void SearchResSelector::setFont()
{
    this->resFont.setFamily("Arial");
    this->resFont.setBold(true);
    this->resFont.setPixelSize(this->text_height);

    QFontMetrics fm(this->resFont);

    int textWidthInPixels = fm.width(this->resString);

    if ( textWidthInPixels > (this->text_width))
    {
         this->resFont.setStretch
                 ((100 * this->text_width) / textWidthInPixels) ;
    }
}

void SearchResSelector::render(QPainter *painter)
{
    QBrush brush(QColor(255,0,0,190));
    QPen pen(QColor(0,0,0,190));

    painter->setBrush(brush);
    painter->setPen(pen);
    this->bsearch->render(painter);
    this->bramble->render(painter);

    painter->setFont(this->resFont);

    painter->setBrush(QBrush(QColor(255,255,255,190)));

    painter->drawRoundedRect(this->r_x, this->r_y, this->r_width,
                             this->r_height, 5, 5);
    painter->drawText(this->text_x, this->text_y, this->text_width,
                      this->text_height, Qt::AlignCenter, this->resString);
}

void SearchResSelector::setResString(QString resStr)
{
    this->resString = resStr;
    this->setFont();
}

void SearchResSelector::processMouseEvent(QMouseEvent *e)
{
    if (e->type() == QEvent::MouseButtonPress)
    {
        bool buttonPressed = false;

        if(this->bsearch->isInside(e->x(), e->y()))
        {
            this->dialogResult = 0;
            buttonPressed = true;
        }
        if(this->bramble->isInside(e->x(), e->y()))
        {
            this->dialogResult = 1;
            buttonPressed = true;
        }

        //only if buttons were not pressed
        if(!buttonPressed)
        {            
            this->mouseActive = true;
        }
    }
    else if(e->type() == QEvent::MouseMove)
    {

    }
    else if(e->type() == QEvent::MouseButtonRelease)
    {
        this->mouseActive = false;
    }
}

SearchResSelector::~SearchResSelector()
{
    delete this->bsearch;
    delete this->bramble;
}

