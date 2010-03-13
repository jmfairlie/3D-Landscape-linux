#include "MenuItem.h"


MenuItem::MenuItem(QString name, QImage * icon, MenuItem * parent, QPointF size, MenuManager * manager)
{
    this->name = name;
    this->icon = icon;
    this->parent = parent;
    this->status = E_HIDDEN;
    this->scale = QPointF(0,0);
    this->position = QPointF(0,0);
    this->size = size;
    this->growthRate = 0.5;
    this->expansionRadius = size.x()*2;
    this->speed = 2.0;
    this->childReadyCount = 0;
    this->numChildren = 0;
    this->children = NULL;
    this->expanded = false;
    this->inAncestorQueue = false;
    this->isCurrent = false;
    this->moveInitiator = NULL;
    this->childOrder = 0;
    this->manager =  manager;
    this->mouseOver = false;
    this->font = QFont();
    this->font.setBold(true);
    if (this->parent)
    {
        this->parent->addChild(this);
    }
}

void MenuItem::hideImmediate()
{
    cout<<this->name.toStdString()<<"  hideImmediate"<<endl;
    for (int i = 0; i< this->numChildren; i++)
    {
        this->children[i]->status = E_HIDDEN;
        cout<<this->children[i]->name.toStdString()<<" "<<this->children[i]->getStatus()<<endl;
    }
    this->status = E_HIDDEN;
    this->expanded = false;
}


void MenuItem::showImmediate(QPointF startpos)
{
    this->position = startpos;
    this->scale = QPointF(1,1);
    if (this->numChildren)
    {
        float angle = 2*PI/this->numChildren;
        for (int i = 0; i< this->numChildren; i++)
        {
            this->children[i]->status = E_ENABLED;
            this->children[i]->position = QPointF(this->position.x()+this->expansionRadius*cos(angle*i),this->position.y()+ this->expansionRadius*sin(angle*i));
            this->children[i]->scale = QPointF(1,1);
        }
        this->expanded = true;
    }

    this->status = E_ENABLED;
}

void MenuItem::resetItem()
{
    this->status = E_HIDDEN;
    this->scale = QPointF(0,0);
    this->position = QPointF(0,0);

}

void MenuItem::resize(QPointF size)
{
    this->size = size;
    this->expansionRadius = size.x()*2;   
}

bool MenuItem::isInAncestorQueue()
{
    return this->inAncestorQueue;
}
void MenuItem::setChildOrder(int order)
{
    this->childOrder = order;
}
void MenuItem::addChild(MenuItem * child)
{
    child->setChildOrder(this->numChildren);
    MenuItem ** auxChildrenList;
    if (numChildren)
    {
        auxChildrenList = new MenuItem *[numChildren];

        for(int i = 0 ; i< numChildren; i++)
        {
            auxChildrenList[i] = this->children[i];
        }

       delete this->children;
    }

    this->children = new MenuItem *[numChildren+1];

    for(int j = 0; j< numChildren; j++)
    {
       this->children[j]= auxChildrenList[j];
    }

    this->children[numChildren] = child;

    numChildren++;

}
MenuItem::~MenuItem()
{

}

bool MenuItem::hitTest(QPoint p)
{
    return (p.x()> this->position.x() - this->size.x()/2*this->scale.x() &&
            p.x()< this->position.x() + this->size.x()/2*this->scale.x() &&
            p.y()> this->position.y() - this->size.y()/2*this->scale.y() &&
            p.y()< this->position.y() + this->size.y()/2*this->scale.y());
}

bool MenuItem::hover(QPointF p)
{
    if(hitTest(p.toPoint()))
    {
      if(this->status == E_ENABLED)
      {
          if(!this->mouseOver)
          {
              this->mouseOver = true;

          }
      }
      return true;

    }
    else
    {
      if(this->mouseOver)
          {
              this->mouseOver = false;

          }
      return false;
    }
}


bool MenuItem::click(QPointF p)
{
    if(hitTest(p.toPoint()))
    {


      //make sure the elements are ready to take events
      if(this->status == E_ENABLED)
      {
          cout<<this->name.toStdString()<<" clicked with status "<<this->status<<endl;
          if(this->expanded)
          {
             if(this->parent)
             {
                 this->initGoBackTranslation();
                 this->manager->setCurrent(this->parent);
             }
             else
             {
                cout<<"init collapse"<<endl;
                this->initCollapse();
             }
          }
          else // clicked one of the children
          {
              if (this->numChildren)
              {
                this->parent->initTranslation(this);
                this->manager->setCurrent(this);
              }

              this->manager->emitClickEvent(this->name);
          }
          return true;
      }

    }
    return false;
}

void MenuItem::initgrow(QPointF startpos)
{
    if (this->status == E_HIDDEN)
    {
        status = E_GROWING;
        this->position = startpos;
        this->scale= QPointF(0,0);
    }
}

void MenuItem::initShrinkage()
{
    if (this->status != E_HIDDEN)
    {
        status = E_SHRINKING;
        this->scale= QPointF(1,1);
    }
}

QPointF MenuItem::getPos()
{
    return this->position;
}

void MenuItem::grow()
{
    if (this->scale.x() + this->growthRate < 1.0)
    {
        this->scale.setX(this->scale.x() + this->growthRate);
        this->scale.setY(this->scale.y() + this->growthRate);
    }
    else
    {
        this->scale.setX(1.0);
        this->scale.setY(1.0);
        this->initExpansion();

        //when initGrow is called by the moving children in its ChildReady
        //we need to make sure that this expansion doesnt include the moveInititor.
        if (this->moveInitiator)
        {
            this->childReady();
        }
    }
}

void MenuItem::shrink()
{
    if (this->scale.x() - this->growthRate > 0.0)
    {
        this->scale.setX(this->scale.x() - this->growthRate);
        this->scale.setY(this->scale.y() - this->growthRate);
    }
    else
    {
        this->scale.setX(0.0);
        this->scale.setY(0.0);
        this->hide();
        this->moveInitiator = NULL;
        this->manager->notifyEndOfShrink(this);
    }
}

void MenuItem::initTranslation(MenuItem * initiator)
{
    //in this case initiator is the child which started the translation
    //and eventually will replace the parent place at the center
    this->moveInitiator = initiator;
    status = E_MOVING;
    QPointF direction = this->position - initiator->getPos();
    this->resetScale();
    this->goToPosition(this->position + direction);
    if (this->numChildren)
    {
        this->childReadyCount = 0;

        for( int i=0; i< this->numChildren; i++)
        {
            QPointF d = this->children[i]->getPos() + direction;
            this->children[i]->resetScale();
            this->children[i]->setInitiator(this);
            this->children[i]->goToPosition(d);
        }
    }
    //this possibility does not happen ever ,  i think
    //because this movement is always started by a child (when clicking a child with children)
    else
    {

    }
}
void MenuItem::resetScale()
{
    this->scale = QPointF(1,1);
}
void MenuItem::initGoBackTranslation()
{
    this->moveInitiator = NULL;
    status = E_MOVING;
    float angle = 2*PI/this->parent->getNumChildren()*this->childOrder;
    QPointF direction = QPointF(this->expansionRadius*cos(angle),this->expansionRadius*sin(angle));
    this->resetScale();
    this->goToPosition(this->position + direction);
    if (this->numChildren)
    {
        this->childReadyCount = 0;
        for( int i=0; i< this->numChildren; i++)
        {
            QPointF d = this->children[i]->getPos() + direction;
            this->children[i]->resetScale();
            this->children[i]->setInitiator(this);
            this->children[i]->goToPosition(d);
        }
    }
}

int MenuItem::getNumChildren()
{
    return this->numChildren;
}

void MenuItem::initExpansion()
{
    status = E_EXPANDING;
    this->childReadyCount = 0;
    if (this->numChildren)
    {
        float angle = 2*PI/this->numChildren;
        for( int i=0; i< this->numChildren; i++)
        {
             if (this->children[i] != this->moveInitiator)
            {
                QPointF d = QPointF(this->position.x()+this->expansionRadius*cos(angle*i),this->position.y()+ this->expansionRadius*sin(angle*i));
                this->children[i]->position = this->position;
                this->children[i]->scale = QPointF(0,0);
                this->children[i]->setInitiator(this);
                this->children[i]->goToPosition(d);
            }
        }
    }
    //if theres no children item should be enabled cause otherwise no one will enable it
    //only applies if the root menu is childless
    else
    {
        this->enable();
    }
}


void MenuItem::initCollapse()
{
    status = E_COLLAPSING;

    this->childReadyCount = 0;


    if (this->numChildren)
    {
        QPointF d = this->position;
        for( int i=0; i< this->numChildren; i++)
        {
            if (this->children[i] != this->moveInitiator)
            {
                this->children[i]->setInitiator(this);
                this->children[i]->goToPosition(d);
            }
        }

    }
    else
    {
        //dont have to wait for children
        //to start shrinking cause theres no one calling
        this->initShrinkage();

    }
}


void MenuItem::goToPosition(QPointF destination)
{
    this->destination = destination;
    this->status = E_MOVING;
    this->velocity = (this->destination - this->position)/this->speed;
}


void MenuItem::move()
{
   if (abs(this->destination.x() - this->position.x() - this->velocity.x()) > 1 || abs(this->destination.y() - this->position.y() - this->velocity.y()) > 1)
   {


       this->position += this->velocity;
       //if it was the parent who started the movement (either expansion or collapse)
       if (this->moveInitiator && (this->moveInitiator == this->parent))
       {
           cout<<this->name.toStdString()<<" move"<<endl;

           QPointF delta = QPointF(1,1)/this->speed;
           if(this->parent->getStatus() == E_EXPANDING)
           {
            this->scale +=delta;
           }
           else if (this->parent->getStatus() == E_COLLAPSING)
           {
               this->scale -=delta;
           }
       }
   }
   else
   {
       this->status = E_DISABLED;
       this->position = this->destination;
       //if it was the parent who started the movement (either expansion or collapse)
       if (this->moveInitiator && this->moveInitiator == this->parent)
       {
            if(this->parent->getStatus() == E_EXPANDING)
            {
                this->scale = QPointF(1,1);

            }
            else if (this->parent->getStatus() == E_COLLAPSING)
            {
                this->scale = QPointF(0,0);
            }

            //if the movement was part of a translation
            else if (this->parent->getStatus() == E_DISABLED)
            {

            }
            this->moveInitiator = NULL;
            this->parent->childReady();

            //might need to add the options of moving up and down on the tree
            //these are also started by the parent
       }
       //if there is no moveinitiator or there is but its the child
       else
       {
           if(this->numChildren )
           {

               if (this->numChildren == this->childReadyCount)
               {

               }
               else
               {
                   //wait for the children to finish their own movements
               }
           }
           //no children so enable
           else
           {
               this->enable();
           }


       }
   }
}


void MenuItem::expand()
{
}

void MenuItem::collapse()
{
}

void MenuItem::update()
{
    switch(this->status)
    {
        case E_DISABLED:
        break;

        case E_ENABLED:
        break;

        case E_HIDDEN:
        break;

        case E_EXPANDING:

        break;

        case E_COLLAPSING:
        break;

        case E_GROWING:
            this->grow();
        break;

        case E_SHRINKING:
            this->shrink();
        break;

        case E_MOVING:
            this->move();
        break;
    }
}

bool MenuItem::childReady()
{
    this->childReadyCount++;

    if (this->childReadyCount == this->numChildren)
    {
        switch (this->status)
        {
            case E_EXPANDING:

                for (int i=0; i<this->numChildren;i++)
                {
                    //dont enable the initiator cause
                    //its probably going to be enabled
                    //by other means
                    if ( this->children[i] != this->moveInitiator)
                    {
                        this->children[i]->enable();
                    }
                }
                this->expanded = true;
                this->moveInitiator = NULL;
                this->enable();
            break;

            case E_COLLAPSING:
                for (int i=0; i<this->numChildren;i++)
                {
                   if (this->children[i] != this->moveInitiator)
                   {
                        this->children[i]->hide();
                   }
                }

                this->expanded = false;
                //movement started by a replacing child
                //or its the root of the menu
                if (this->moveInitiator || !this->parent)
                {
                    this->initShrinkage();
                }
                //since it doesnt need to shrink
                //it needs to be enabled
                else
                {
                    this->enable();
                }

            break;

            case E_DISABLED: //just finished one of the translations

                    this->initCollapse();

                    if (this->moveInitiator) // move because of clicking on one child
                    {
                        //this counts for the initiator which wont send the child ready message
                        //cause its actually not moving
                        this->childReady();
                        this->moveInitiator->initExpansion();
                    }
                    else //move because of clicking on central item
                    {

                        float angle = 2*PI/this->parent->getNumChildren()*this->childOrder;
                        QPointF d = QPointF(this->position.x()-this->expansionRadius*cos(angle),this->position.y()- this->expansionRadius*sin(angle));
                        this->parent->moveInitiator = this;
                        this->parent->initgrow(d);
                    }

            break;
            default:

            break;
        }
    }
    return false;
}
void MenuItem::enable()
{
    this->status = E_ENABLED;
}

void MenuItem::disable()
{
    this->status = E_DISABLED;
}

void MenuItem::hide()
{
    this->status = E_HIDDEN;
}

void MenuItem::render(QPainter * painter)
{

    if (this->status != E_HIDDEN)
    {
        painter->save();

        float caca=1.0;
        if (this->mouseOver)
        {

            caca = 1.3;
        }

        QPointF  currSize = QPointF(this->size.x()*this->scale.x()*caca,this->size.y()*this->scale.y()*caca);
        if (this->icon)
        {
            QRectF rect = QRectF(this->position.x()-currSize.x()/2,this->position.y()-currSize.y()/2,currSize.x(), currSize.y());
            painter->drawImage(rect,*this->icon);
        }
        else
        {
           painter->drawEllipse(this->position, (int)currSize.x()/2, (int)currSize.y()/2);
        }
        if (this->status == E_ENABLED)
        {
            painter->setPen(QColor(0,0,0,255));
            QTextOption to = QTextOption();
            this->font.setPixelSize(currSize.x()/5);
            painter->setFont(this->font);

            to.setAlignment(Qt::AlignHCenter);
            QPointF tr = this->position+ QPointF(-currSize.x(),currSize.y()*0.5);
            QPointF bl = tr + QPointF(currSize.x()*2,currSize.y()/4);
            painter->drawText(QRectF(tr,bl),this->name,to);
        }
        painter->restore();
    }
}

int MenuItem::getStatus()
{
    return this->status;
}

void MenuItem::setInitiator(MenuItem* initiator)
{
    this->moveInitiator = initiator;
}


