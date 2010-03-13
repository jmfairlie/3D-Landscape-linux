#include "MenuManager.h"

MenuManager::MenuManager()
{
    this->numItems = 0;
    this->numIcons = 0;
    this->menuItems = NULL;
    this->iconList = NULL;
    //default it should be changed later with resize events
    this->size = QPointF(1024,768);
    this->center = size/2;
    this->createMenu();
    this->bgoffset = 0;
    this->bglocked = true;
    this->bgdescending = true;
    this->moveratio = 0.8;
}
void MenuManager::resize(int width, int height)
{
    this->size = QPointF(width,height);
    this->center = size/2;

    bool wasOpen =  this->currentItem->getStatus() != E_HIDDEN;
    this->resizeItems(QPointF(height/10,height/10));
    if (wasOpen)
    {
        this->showMenuImmediate();
    }

}

void MenuManager::emitClickEvent(QString name)
{
    emit this->clicked(name);
}

void MenuManager::resizeItems(QPointF newItemSize)
{
    for(int i=0; i<this->numItems; i++)
    {
        this->menuItems[i]->resize(newItemSize);
        this->menuItems[i]->resetItem();

    }
}

void MenuManager::processMouseEvent(QMouseEvent *e)
{
    switch (e->type())
    {
        case  QEvent::MouseButtonPress:
            for(int i=0; i<this->numItems; i++)
            {
                if (this->menuItems[i]->click(e->posF()))
                {
                    //emit clicked signal
                    emit this->clicked(this->menuItems[i]->name);
                    //break;
                }
            }
        break;

        case  QEvent::MouseMove:
            for(int i=0; i<this->numItems; i++)
            {
                if (this->menuItems[i]->hover(e->posF()))
                {
                    //break;
                }
            }
        break;
        default:
        break;
    }
}

MenuManager::~MenuManager()
{
    for(int i=0; i<this->numItems; i++)
    {
        delete this->menuItems[i];

    }
    delete[] this->menuItems;



    for(int i=0; i<this->numIcons; i++)
    {

        delete this->iconList[i];
    }

    delete[] this->iconList;
}

void MenuManager::renderMenu(QPainter* painter)
{

    this->renderBg(painter);
    for(int i=0; i<this->numItems; i++)
    {
        this->menuItems[i]->render(painter);
    }
}

void MenuManager::renderBg(QPainter* painter)
{
    //if(this->currentItem->getStatus() != E_HIDDEN)
    {
        if (!bglocked)
        {
            if (this->bgdescending)
            {
                if (this->bgoffset + (1-this->bgoffset)*this->moveratio < (1 - Utils3D::TOLERANCE))
                {
                    cout<<this->bgoffset<<endl;
                    this->bgoffset+= (1 -this->bgoffset)*this->moveratio;
                }
                else
                {

                    this->bglocked = true;
                    this->bgoffset = 1.0;
                    this->currentItem->initgrow(this->center);
                    cout<<"cayo el telon"<<endl;
                }
            }
            else
            {
                if (this->bgoffset - (this->bgoffset)*this->moveratio > Utils3D::TOLERANCE)
                {
                    cout<<this->bgoffset<<endl;
                    this->bgoffset-= (this->bgoffset)*this->moveratio;

                }
                else
                {
                    this->bglocked = true;
                    this->bgoffset = 0.0;
                    cout<<"se levanto el telon"<<endl;
                }
            }
        }

        int bgwidth = this->size.y()/1.5;
        int bgheight = bgwidth*0.9;
        int posx = this->center.x()- bgwidth/2;
        int posy = -bgheight + this->bgoffset*(this->size.y()/2 + bgheight/2 );

        QPen p = QPen(QColor(255,255,255,180));
        p.setWidth(5);
        painter->setPen(p);

        QBrush b = QBrush(QColor(255,255,255,150));
        painter->setBrush(b);
        painter->drawRoundRect(posx, posy ,bgwidth,bgheight,25,25);
    }
}

void MenuManager::updateMenu()
{
    for(int i=0; i<this->numItems; i++)
    {
        this->menuItems[i]->update();
    }
}

void MenuManager::createMenu()
{
    /*
    int hijos = 1;
    int nietosporhijo = 1;
    int bisnietospornieto = 1;
    */
    int menusize = 70;
    this->numItems = 11;//hijos*nietosporhijo*bisnietospornieto + hijos*nietosporhijo + hijos + 1;
    this->numIcons = 5;

    this->iconList = new QImage *[this->numIcons];

    this->iconList[0] = new QImage("../res/images/png/HP-Blank-Folder-Dock.png");
    this->iconList[1] = new QImage("../res/images/png/HP-MSN-Dock.png");
    this->iconList[2] = new QImage("../res/images/png/HP-Firefox-Dock.png");
    this->iconList[3] = new QImage("../res/images/png/HP-Control-Dock.png");
    this->iconList[4] = new QImage("../res/images/png/HP-Recycle-Empty-Dock.png");


    this->menuItems = new MenuItem *[this->numItems];

    this->menuItems[0] = new MenuItem(QString("Menu"),this->iconList[0],NULL, QPointF(menusize,menusize),this);
    this->currentItem = this->menuItems[0];


    /*
    for(int j=0; j<hijos; j++)
    {
        QString n;
        n.sprintf("%i.%i",0, j+1);
        menuItems[1 + j + j*nietosporhijo + j*nietosporhijo*bisnietospornieto] = new MenuItem(n, this->iconList[1], this->menuItems[0], QPointF(menusize,menusize),this);

        for(int i=0; i<nietosporhijo; i++)
        {
            QString n;
            n.sprintf("%i.%i.%i",0,j+1,i+1);
            menuItems[1 + j + j*nietosporhijo + j*nietosporhijo*bisnietospornieto + 1 + i + i*bisnietospornieto] = new MenuItem(n, this->iconList[2], this->menuItems[1 + j + j*nietosporhijo + j*nietosporhijo*bisnietospornieto], QPointF(menusize,menusize),this);

            for(int k=0; k<bisnietospornieto; k++)
            {
               QString n;
                n.sprintf("%i.%i.%i.%i",0,j+1,i+1,k+1);
                menuItems[1 + j + j*nietosporhijo + j*nietosporhijo*bisnietospornieto + 1 + i + i*bisnietospornieto + 1 + k] = new MenuItem(n, this->iconList[3], this->menuItems[1 + j + j*nietosporhijo + j*nietosporhijo*bisnietospornieto + 1 + i + i*bisnietospornieto], QPointF(menusize,menusize),this);

            }

        }
    }

    */

    menuItems[1] = new MenuItem(QString("Search"),this->iconList[1],this->menuItems[0], QPointF(menusize,menusize),this);
    menuItems[2] = new MenuItem(QString("From Favorites"),this->iconList[2],menuItems[1], QPointF(menusize,menusize),this);
    menuItems[3] = new MenuItem(QString("New"),this->iconList[2],menuItems[1], QPointF(menusize,menusize),this);

    menuItems[4] = new MenuItem(QString("Movie Theaters"),this->iconList[3],menuItems[3], QPointF(menusize,menusize),this);
    menuItems[5] = new MenuItem(QString("Restaurants"),this->iconList[3],menuItems[3], QPointF(menusize,menusize),this);
    menuItems[6] = new MenuItem(QString("Bars/Clubs"),this->iconList[3],menuItems[3], QPointF(menusize,menusize),this);
    menuItems[7] = new MenuItem(QString("Name"),this->iconList[4],menuItems[6], QPointF(menusize,menusize),this);
    menuItems[8] = new MenuItem(QString("Distance"),this->iconList[4],menuItems[6], QPointF(menusize,menusize),this);
    menuItems[9] = new MenuItem(QString("Fees"),this->iconList[4],menuItems[6], QPointF(menusize,menusize),this);
    menuItems[10] = new MenuItem(QString("Activities"),this->iconList[4],menuItems[6], QPointF(menusize,menusize),this);

}

void MenuManager::hideMenuImmediate()
{
    if(this->currentItem->getStatus() != E_HIDDEN)
    {
        this->currentItem->hideImmediate();
    }
    this->bgoffset = 0;
    this->bglocked = true;
}


void MenuManager::showMenuImmediate()
{
    if(this->currentItem->getStatus() == E_HIDDEN)
    {
        this->currentItem->showImmediate(this->center);
    }
    this->bgoffset = 1;
    this->bglocked = true;
}

void MenuManager::setRoot()
{
    this->currentItem = this->menuItems[0];
}

void MenuManager::showHideMenu()
{
    if(this->currentItem->getStatus() == E_HIDDEN)
    {
        this->bglocked = false;
        this->bgdescending = true;
        this->bgoffset = 0.0;

    }
    else if (this->currentItem->getStatus() == E_ENABLED)
    {

        this->currentItem->initCollapse();
        //dirty trick to make sure the menu dissapears when current item
        //is not the root
        this->currentItem->setInitiator(this->currentItem);
    }
}


void MenuManager::notifyEndOfShrink(MenuItem* item)
{
    if (this->currentItem == item)
    {
        this->bglocked = false;
        this->bgoffset = 1.0;
        this->bgdescending = false;
    }
}

MenuItem* MenuManager::getCurrent()
{
    return this->currentItem;
}

void MenuManager::setCurrent(MenuItem* item)
{
    this->currentItem = item;
}
