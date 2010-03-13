#include "Popup.h"

 Popup::Popup()
 {
     this->growthFactor = 0.8;
     this->scale = 0;
     this->status = E_PHIDDEN;
     this->tailRatio = QPointF(0.25,0.1);
     this->screenSizeRatio = 0.4;
 }

 void Popup::resize(float w, float h)
 {
     this->size = QPointF(w*this->screenSizeRatio,h*this->screenSizeRatio);
 }

 void Popup::show()
 {

 }

 void Popup::hide()
 {
    this->status = E_PHIDDEN;
 }

 void Popup::update()
 {
    switch(this->status)
    {
        case E_PHIDDEN:
        break;

        case E_PGROWING:
            this->grow();
        break;

        case E_PSHRINKING:
            this->shrink();
        break;

        case E_PENABLED:
        break;

        default:
        break;
    }
 }

 void Popup::grow()
 {
     float delta = (1- this->scale)*this->growthFactor;

     if (this->scale + delta  < 1.0 - Utils3D::TOLERANCE)
     {
         this->scale += delta;
         cout<<"pop scale: "<<this->scale<<endl;
     }
     else
     {
         this->scale = 1.0;
         this->status = E_PENABLED;
     }
 }

 void Popup::shrink()
 {
     float delta = this->scale*this->growthFactor;

     if (this->scale - delta  > Utils3D::TOLERANCE)
     {
         this->scale -= delta;
         cout<<"pop scale: "<<this->scale<<endl;
     }
     else
     {
         this->scale = 0.0;
         this->hide();
     }
 }

void Popup::startGrowth()
{
    if (this->status == E_PHIDDEN)
    {
        cout<<"start shrinkage"<<endl;
        this->status = E_PGROWING;
    }
}

void Popup::startShrinkage()
{
     if (this->status == E_PENABLED)
    {
        cout<<"start shrinkage"<<endl;
        this->status = E_PSHRINKING;
    }
}

bool Popup::click(QMouseEvent* e)
{
    if (this->status == E_PENABLED && this->box.contains(e->x(), e->y()))
    {
        this->startShrinkage();
        return true;
    }
    else
        return false;
}

void Popup::render(QPainter* p,QPointF pos,QString dstName)
{
    if (this->status != E_PHIDDEN)
    {
       QPointF currentSize = this->size*this->scale;
       QPointF currentTail = QPointF(this->tailRatio.x()*currentSize.x(), this->tailRatio.y()*currentSize.x());

       QPen pen(QColor(50,50,50,255*this->scale));
       pen.setWidthF(this->size.x()*this->scale/30);

       p->setPen(pen);

       QPointF currentBox = currentSize-currentTail;
       float offset = currentBox.y()/5;
       QPointF points[4];
       points[0] = QPointF(pos.x(),pos.y());
       points[1] = QPointF(pos.x() + currentTail.x(),pos.y() - currentTail.y() - offset);
       points[2] = QPointF(pos.x() + currentTail.x(),pos.y() - currentTail.y());
       points[3] = QPointF(pos.x() + currentTail.x() + offset,pos.y() - currentTail.y() );
       p->setBrush(QBrush(QColor(255,255,255,200*this->scale)));
       p->drawRoundedRect(pos.x()+ currentTail.x(),pos.y()-currentSize.y(), currentBox.x(),currentBox.y(),10,10);
       pen.setWidth(0);
       p->setPen(pen);
       p->setBrush(QBrush(QColor(50,50,50,255*this->scale)));
       p->drawPolygon(points, 4);

       //update the box;
       this->box.setX(pos.x()+ currentTail.x());
       this->box.setY(pos.y()-currentSize.y());
       this->box.setWidth(currentBox.x());
       this->box.setHeight(currentBox.y());

       //text
       this->font.setPixelSize(currentBox.x()/10);
       p->setFont(this->font);

       QPointF tr = QPointF(pos.x()+ currentTail.x(),pos.y()-currentSize.y());
       QPointF bl = tr + currentBox;

       p->drawText(QRectF(tr,bl), Qt::AlignCenter, dstName);
   }
}


