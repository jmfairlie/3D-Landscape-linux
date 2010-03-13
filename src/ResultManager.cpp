#include "ResultManager.h"

ResultManager::ResultManager(Engine3D *engine3D)
{
    this->engine3D = engine3D;
}

void ResultManager::render(QPainter *painter)
{
    for(int i = 0; i < this->engine3D->foundDestinations.size(); i++)
    {
        //project the point to the screen

        GLdouble xd, yd, zd;

        glMatrixMode(GL_MODELVIEW);

        glPushMatrix();
            this->engine3D->setSearchCamera();

            GLdouble modelMatrix[16];
            glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
        glPopMatrix();

            glMatrixMode(GL_PROJECTION);
        glPushMatrix();
            glLoadIdentity();
            glFrustum(this->engine3D->fleft, this->engine3D->fright,
                      this->engine3D->fbottom, this->engine3D->ftop, 
                      this->engine3D->fnear, this->engine3D->ffar);

            GLdouble projMatrix[16];
            glGetDoublev(GL_PROJECTION_MATRIX,projMatrix);

            glViewport(0,0,(GLsizei)this->engine3D->screenw,
                       (GLsizei)this->engine3D->screenh);
            int viewport[4];
            glGetIntegerv(GL_VIEWPORT,viewport);
       glPopMatrix();

       gluProject(
                this->engine3D->foundDestinations[i]->coords.x,
                this->engine3D->foundDestinations[i]->coords.y,
                this->engine3D->foundDestinations[i]->coords.z,
                modelMatrix,
                projMatrix,
                viewport,
                &xd,
                &yd,
                &zd);
       this->resultItems[i]->render(painter,
            xd, this->engine3D->height() - yd);
   }
}

void ResultManager::processMouseClick(int x, int y)
{
    for(int i = 0; i < this->resultItems.size(); i++)
    {
        if(this->resultItems[i]->isInside(x, y))
        {
            if(i < this->engine3D->foundDestinations.size())
            {
                cout << "Clicked destination name: " <<
                    this->engine3D->foundDestinations[i]->name << endl;

                this->engine3D->menuManager->hideMenuImmediate();
                this->engine3D->menuManager->setRoot();
                this->engine3D->startDescensionToDestination(
                       this->engine3D->foundDestinations[i]->coords,
                       this->engine3D->foundDestinations[i]->nodeCoords);
                break;
            }
        }
    }
}

void ResultManager::addChild(QString label)
{
    QSize mainWidgetSize = this->engine3D->size();
    this->resultItems.append(new ResultItem(
            mainWidgetSize.height()/20, mainWidgetSize.height()/20, label));
}

void ResultManager::resize(QSize mainWidgetSize)
{
    for(int i = 0; i < this->resultItems.size(); i++)
    {
        this->resultItems[i]->width = mainWidgetSize.height()/10;
        this->resultItems[i]->height = mainWidgetSize.height()/10;
    }
}

void ResultManager::deleteItems()
{
    for(int i = 0; i < this->resultItems.size(); i++)
    {
        delete this->resultItems[i];
    }
    this->resultItems.clear();
}

ResultManager::~ResultManager()
{
    this->deleteItems();
}

