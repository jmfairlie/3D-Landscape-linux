#ifndef Widget3D_H
#define Widget3D_H

#include <qgl.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <QGLWidget>
#include <QFile>

#ifdef __WIN32__
#include "ffmpegreader.h"
#include "ffvideothread.h"
#endif

#define SAME_CLOCKNESS 1
#define DIFF_CLOCKNESS 0

struct Point3D
{
    float x, y, z;

    Point3D();
    Point3D(float x, float y, float z);
};

class Widget3D
{
public:
    Point3D tl, bl, tr, br;

    bool isSelected;

public:
    Widget3D(Point3D tl, Point3D bl, Point3D tr, Point3D br);
    Widget3D(Point3D tl, Point3D bl, Point3D tr, Point3D br, GLuint id,
             QWidget *widget, bool isWideoWidget = false);

    Point3D getLocalCoord();

    Point3D interPoint;

    virtual ~Widget3D();

    void render() const;

    bool intersects(Point3D nearr, Point3D farr);

    void changeSelection(bool s);

    QWidget * widget;
    //QWidget * widgetUnder;
    GLuint imageIndex;

#ifdef __WIN32__
    //whether widget is video player
    bool isVideoWidget;
    bool isVideoStarted;
    void startVideo();
    void stopVideo();
    FFVideoThread *ffVideoThread;
    FFmpegReader *ffMpegReader;
#endif

private:
    int check_same_clock_dir(Point3D pt1, Point3D pt2, Point3D pt3, Point3D norm);
    int check_intersect_tri(Point3D pt1, Point3D pt2, Point3D pt3, Point3D linept, Point3D vect,
                        Point3D* pt_int);


};

#endif // Widget3D_H
