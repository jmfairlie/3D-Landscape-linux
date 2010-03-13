#include "imagewidget.h"

ImageWidget::ImageWidget(QString pathToImages, QWidget *parent)
       : QWidget(parent)
{
    this->insertionPointX = 0;
    this->showOffsetX = 0;
    this->velocityX = 0.0f;
    this->mouseActive = false;
    this->pressedCoordX = 0;
    this->startTime = 0;
    this->previousX = 0;
    this->currentIndex = 0;

    this->currSize.setWidth(512);
    this->currSize.setHeight(512);

    this->mode = E_Browse;

    this->directory = new QDir(pathToImages);
    this->directory->setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QStringList filters;
    filters << "*.jpg" << "*.gif" << "*.png" << "*.bmp";
    this->directory->setNameFilters(filters);

    QFileInfoList list = this->directory->entryInfoList();    

    for(int i = 0; i < list.size(); i++)
    {
        //cout << list[i].absoluteFilePath().toStdString() << endl;
        QPixmap newPM(list[i].absoluteFilePath());        

        if(!newPM.isNull())
        {
            this->pixmaps.append(newPM);
            QPixmap scaledTmp = newPM.scaled(IMGSIZE, IMGSIZE);
            this->pixmapscaled.append(scaledTmp);            
        }
    }

    this->pixbuffer = new QPixmap(IMGSIZESP*this->pixmapscaled.size(), IMGSIZE);

    this->setFixedSize(this->currSize.width(), this->currSize.height());

    QPainter paintPixbuffer(this->pixbuffer);

    //cout << "scaled num: " << this->pixmapscaled.size() << endl;

    for(int i = 0; i < this->pixmapscaled.size(); i++)
    {
        paintPixbuffer.drawPixmap(this->insertionPointX, 0, this->pixmapscaled[i]);
        this->insertionPointX += IMGSIZESP;
    }

    //enable mouse tracking
    this->setMouseTracking(true);

    this->m_timer = new QTimer( this );
    connect(this->m_timer, SIGNAL(timeout()), this, SLOT(update()));
    this->m_timer->start(42);

    if(this->pixmaps.size() > 0)
        this->currentIndex = 0;
    else
        this->currentIndex = -1;
}

void ImageWidget::mouseDoubleClickEvent( QMouseEvent *e )
{
    if(this->mode == E_Browse)
    {
        this->mode = E_View;

        this->velocityX = 0.0f;

        //get clicked image index
        //cout << "index: " << (e->x() + this->showOffsetX) / IMGSIZESP << endl;
        this->currentIndex = (e->x() + this->showOffsetX) / IMGSIZESP;

    }
    else
    {
        this->mode = E_Browse;
    }
}

void ImageWidget::mousePressEvent( QMouseEvent *e )
{
    this->mouseActive = true;
    this->pressedCoordX = e->x();
    this->previousX = e->x();

    this->time.start();
    this->startTime = this->time.elapsed();
}

void ImageWidget::mouseReleaseEvent(QMouseEvent *e)
{
    this->mouseActive = false;

    int deltaXtmp = this->pressedCoordX - e->x();

    this->velocityX = 10.0f*((float)deltaXtmp/(float)(this->time.elapsed() - this->startTime));

    //cout << this->velocityX << endl;
    //this->updateOffset(deltaXtmp);
}

void ImageWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(this->mouseActive)
    {
        int deltaXtmp = this->previousX - e->x();
        this->previousX = e->x();

        this->updateOffset(deltaXtmp);
    }
}

void ImageWidget::updateOffset(float delta)
{
    int newOffsetX = (int)(this->showOffsetX + delta);

    if((newOffsetX > 0) && (newOffsetX < (this->pixmapscaled.size()*IMGSIZESP - IMGSTRIPLEN*IMGSIZESP)))
    {
        this->showOffsetX = newOffsetX;
    }
    else if(newOffsetX <= 0)
    {
        this->showOffsetX = 0;
        this->velocityX = 0.0f;
    }
    else if(newOffsetX >= (this->pixmapscaled.size()*IMGSIZESP - IMGSTRIPLEN*IMGSIZESP))
    {
        this->showOffsetX = this->pixmapscaled.size()*IMGSIZESP - IMGSTRIPLEN*IMGSIZESP;
        this->velocityX = 0.0f;
    }
}

void ImageWidget::paintEvent(QPaintEvent * pe)
{
    this->insertionPointX = 0;

    QPainter paint(this);         // paint widget

    if(this->mode == E_Browse)
    {
        if(this->pixbuffer)
        {            

            if (this->pixmaps.size() > 0)
            {
                this->updateOffset(this->velocityX);

                paint.fillRect(0,0,this->width(), this->height(),QColor(0,0,0,255));
                paint.drawPixmap(0, (this->currSize.height() - IMGSIZE) / 2,
                                 *this->pixbuffer, this->showOffsetX,
                                 0, IMGSIZESP*IMGSTRIPLEN, IMGSIZE);


                if(fabs(this->velocityX) > 1.0f)
                {
                    this->velocityX -= 0.1 *(this->velocityX/fabs(this->velocityX));
                    //cout << "current velocity " << this->velocityX << endl;
                }
                else
                    this->velocityX = 0.0f;
            }
        }
    }
    else
    {
        if(this->pixmaps.size() >= this->currentIndex + 1)
        {
            QPixmap toDraw = this->pixmaps[this->currentIndex].scaled(this->currSize.width(),
                                                                      this->currSize.height());
            paint.drawPixmap(0, 0, toDraw);
        }
    }
}

ImageWidget::~ImageWidget()
{
    delete this->directory;
    delete this->pixbuffer;
}

