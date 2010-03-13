#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QImage>
#include <QPaintEvent>
#include <QPainter>
#include <iostream>
#include <QDir>
#include <QList>
#include <QMouseEvent>
#include <QTimer>
#include <QTime>
#include <cmath>

#define IMGSTRIPLEN 5
#define IMGSIZE 128
//image size with spacing
#define IMGSIZESP (IMGSIZE + 2)

using namespace std;

enum EMode
{
    E_Browse = 1001,
    E_View
};

class ImageWidget : public QWidget
{    
protected:
    void paintEvent(QPaintEvent * pe);
    void mousePressEvent(QMouseEvent *e );
    void mouseReleaseEvent(QMouseEvent *e );
    void mouseMoveEvent(QMouseEvent *e);
    void mouseDoubleClickEvent( QMouseEvent *e );
private:
    QDir *directory;

    QTime time;

    int currentIndex;

    QList<QPixmap> pixmaps;
    QList<QPixmap> pixmapscaled;

    QTimer *m_timer;

    bool mouseActive;

    EMode mode;

    int insertionPointX;

    //offset in buffer
    int showOffsetX;

    int pressedCoordX;

    float velocityX;

    int startTime;

    int previousX;

    QPixmap *pixbuffer;

    QSize currSize;

    void updateOffset(float delta);
public:
    ImageWidget(QString pathToImages, QWidget *parent=0);

    virtual ~ImageWidget();
};

#endif // IMAGEWIDGET_H
