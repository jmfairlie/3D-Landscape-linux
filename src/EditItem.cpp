#include "EditItem.h"

EditItem::EditItem(QString label, int x, int y, int w, int h, int maxLen)
{
    this->label = label;
    this->currText = "";
    this->x = x;
    this->y = y;
    this->width = w;
    this->height = h;
    this->maxLen = maxLen;

    this->hasFocus = false;

    this->spacing = 2;

    if(w && h)
        this->calculateInnerPositions();
}

//if inside container, should be called after adding to this container
//inside container's initialize function
void EditItem::calculateInnerPositions()
{
    this->title_size = (int)(this->height * 0.2);
    this->x_title = this->x + this->spacing;
    this->y_title = this->y + this->spacing;
    this->width_title = this->width - this->spacing*2;
    this->height_title = this->title_size - this->spacing*2;

    this->titleFont.setFamily("Arial");
    this->titleFont.setPixelSize(this->height_title);
    this->titleFont.setStretch(100);

    QFontMetrics fm(this->titleFont);

    int textWidthInPixels = fm.width(this->label);

    if (textWidthInPixels > (this->width_title))
    {
         this->titleFont.setStretch
                 ((100 * this->width_title) / textWidthInPixels) ;
    }


    //inner rectangle to enter text
    this->h_inner = this->height - this->title_size - this->spacing*3;
    this->w_inner = this->width - this->spacing*3;
    this->y_inner = this->y + this->title_size + this->spacing;
    this->x_inner = this->x + this->spacing;    

    this->y_text = this->y_inner + this->spacing;
    this->x_text = this->x_inner + this->spacing;

    this->height_text = this->h_inner - this->spacing*2;
    this->width_text = this->w_inner - this->spacing*2;

    //text font
    this->setTextFont();
}

void EditItem::setTextFont()
{
    this->textFont.setFamily("Arial");
    this->textFont.setPixelSize(this->height_text / 2);
    this->textFont.setStretch(100);

    QFontMetrics fm(this->textFont);

    int textWidthInPixels = fm.width(this->currText);

    if (textWidthInPixels > (this->width_text))
    {
         this->textFont.setStretch
                 ((100 * this->width_text) / textWidthInPixels) ;
    }
}

//returns true, if user clicked inside the inner rectangle
bool EditItem::isInside(int x, int y)
{
    if(x >= this->x_inner &&
       x <= (this->x_inner + this->w_inner) &&
       y >= this->y_inner &&
       y <= (this->y_inner + this->h_inner))
    {
        return true;
    }
    else
        return false;
}

void EditItem::processKeyEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Backspace)
    {
        if(this->currText.length() != 0)
        {
            this->currText.chop(1);
        }
    }
    else
    {
        if(e->text() != "" && this->currText.length() < this->maxLen)
        {
            this->currText += e->text();
        }
    }

    this->setTextFont();
}

void EditItem::render(QPainter *painter)
{
    //main rectangle
    QBrush brush(QColor(255, 255, 255, 200));
    QPen pen(QColor(0, 0, 0, 255));

    painter->setBrush(brush);
    painter->setPen(pen);
    painter->setFont(this->titleFont);

    //painter->setFont(QFont("Arial", this->height / (this->buttonLabel.length())));
    painter->drawRoundedRect(this->x, this->y, this->width, this->height, 5, 5);

    painter->drawText(this->x_title, this->y_title, this->width_title, this->height_title,
                      Qt::AlignCenter, this->label);

    painter->setBrush(QBrush(Qt::NoBrush));
    painter->setFont(this->textFont);

    painter->drawText(this->x_text, this->y_text, this->width_text, this->height_text,
                      Qt::AlignCenter, this->currText);

    if(this->hasFocus)
    {
        pen.setWidth(2);
        painter->setPen(pen);
    }

    painter->drawRect(this->x_inner, this->y_inner, this->w_inner, this->h_inner);
    painter->setPen(QPen());

}

