#include "Widget3D.h"

Point3D::Point3D(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Point3D::Point3D()
{
}


Widget3D::Widget3D(Point3D tl, Point3D bl, Point3D tr, Point3D br)
{
    this->tl = tl;
    this->bl = bl;
    this->tr = tr;
    this->br = br;

    this->isSelected = false;
    this->isVideoWidget = false;

    this->ffMpegReader = NULL;
    this->ffVideoThread = NULL;
    this->isVideoStarted = false;
}

Widget3D::Widget3D(Point3D tl, Point3D bl, Point3D tr, Point3D br, GLuint imageIndex,
                   QWidget *widget, bool isVideoWidget)
{
    this->tl = tl;
    this->bl = bl;
    this->tr = tr;
    this->br = br;

    this->isSelected = false;

    this->imageIndex = imageIndex;

    this->widget = widget;

    this->isVideoWidget = isVideoWidget;

    this->ffMpegReader = NULL;
    this->ffVideoThread = NULL;
    this->isVideoStarted = false;
}

void Widget3D::changeSelection(bool sel)
{
    this->isSelected = sel;
}

void Widget3D::render() const
{
    /*if(this->isSelected)
        glColor3d(1,0,0);
    else
        glColor3d(0,1,0);*/

    //glGenTextures(1, &this->textureID);

    //glBindTexture(GL_TEXTURE_2D, this->textureID);

    glBegin(GL_LINE_LOOP);

    glVertex3f(this->bl.x,this->bl.y,this->bl.z);
    glVertex3f(this->tl.x,this->tl.y,this->tl.z);
    glVertex3f(this->tr.x,this->tr.y,this->tr.z);

    //glVertex3f(this->bl.x,this->bl.y,this->bl.z);
    //glVertex3f(this->tr.x,this->tr.y,this->tr.z);
    glVertex3f(this->br.x,this->br.y,this->br.z);

    glEnd();
}

bool Widget3D::intersects(Point3D nearr, Point3D farr)
{
    /*printf("near: x, y, z: (%.2f, %.2f, %.2f)\n", lineVec.x, lineVec.y, lineVec.z);
    printf("far: x, y, z: (%.2f, %.2f, %.2f)\n", lineVec.x, lineVec.y, lineVec.z);*/

    Point3D lineVec;

    lineVec.x = farr.x - nearr.x;
    lineVec.y = farr.y - nearr.y;
    lineVec.z = farr.z - nearr.z;

    double lineVecLength = sqrt(lineVec.x*lineVec.x + lineVec.y*lineVec.y + lineVec.z*lineVec.z);

    lineVec.x /= lineVecLength;
    lineVec.y /= lineVecLength;
    lineVec.z /= lineVecLength;

    //printf("Line: x, y, z: (%.2f, %.2f, %.2f)\n", lineVec.x, lineVec.y, lineVec.z);

    bool bintersects = false;

    //first triangle
    bintersects |= this->check_intersect_tri(this->bl, this->tl, this->tr, nearr, lineVec, &(this->interPoint));

    //second triangle
    bintersects |= this->check_intersect_tri(this->tr, this->br, this->bl, nearr, lineVec, &(this->interPoint));

    //printf("inter point x, y: %.2f, %.2f\n", this->interPoint.x, this->interPoint.y);

    return bintersects;
}


int Widget3D::check_same_clock_dir(Point3D pt1, Point3D pt2, Point3D pt3, Point3D norm)
{
   float testi, testj, testk;
   float dotprod;
   // normal of trinagle
   testi = (((pt2.y - pt1.y)*(pt3.z - pt1.z)) - ((pt3.y - pt1.y)*(pt2.z - pt1.z)));
   testj = (((pt2.z - pt1.z)*(pt3.x - pt1.x)) - ((pt3.z - pt1.z)*(pt2.x - pt1.x)));
   testk = (((pt2.x - pt1.x)*(pt3.y - pt1.y)) - ((pt3.x - pt1.x)*(pt2.y - pt1.y)));

   // Dot product with triangle normal
   dotprod = testi*norm.x + testj*norm.y + testk*norm.z;

   //answer
   if(dotprod < 0)
       return DIFF_CLOCKNESS;
   else
       return SAME_CLOCKNESS;
}

int Widget3D::check_intersect_tri(Point3D pt1, Point3D pt2, Point3D pt3, Point3D linept, Point3D vect,
                        Point3D* pt_int)
{
   float V1x, V1y, V1z;
   float V2x, V2y, V2z;
   Point3D norm;
   float dotprod;
   float t;

   // vector form triangle pt1 to pt2
   V1x = pt2.x - pt1.x;
   V1y = pt2.y - pt1.y;
   V1z = pt2.z - pt1.z;

   // vector form triangle pt2 to pt3
   V2x = pt3.x - pt2.x;
   V2y = pt3.y - pt2.y;
   V2z = pt3.z - pt2.z;

   // vector normal of triangle
   norm.x = V1y*V2z-V1z*V2y;
   norm.y = V1z*V2x-V1x*V2z;
   norm.z = V1x*V2y-V1y*V2x;

   // dot product of normal and line's vector if zero line is parallel to triangle
   dotprod = norm.x*vect.x + norm.y*vect.y + norm.z*vect.z;

   //printf("dotprod %.2f\n", dotprod);

   if(dotprod)
   {
      //Find point of intersect to triangle plane.
      //find t to intersect point
      t = -(norm.x*(linept.x-pt1.x)+norm.y*(linept.y-pt1.y)+norm.z*(linept.z-pt1.z))/
             (norm.x*vect.x+norm.y*vect.y+norm.z*vect.z);

      //printf("t %.2f\n", t);
      // if ds is neg line started past triangle so can't hit triangle.
      if(t < 0)
      {
          //printf("t < 0\n");
          return 0;
      }

      pt_int->x = linept.x + vect.x*t;
      pt_int->y = linept.y + vect.y*t;
      pt_int->z = linept.z + vect.z*t;

      if(check_same_clock_dir(pt1, pt2, *pt_int, norm) == SAME_CLOCKNESS)
      {
         if(check_same_clock_dir(pt2, pt3, *pt_int, norm) == SAME_CLOCKNESS)
         {
            if(check_same_clock_dir(pt3, pt1, *pt_int, norm) == SAME_CLOCKNESS)
            {
               // answer in pt_int is inside triangle
               return 1;
            }
            //printf("not same clockness 1\n");
         }
         //printf("not same clockness 2\n");
      }
      //printf("not same clockness 3\n");*/

   }
   return 0;
}

Point3D Widget3D::getLocalCoord()
{
    //vector from the top left to the intersection point
    Point3D tli;
    tli.x = this->interPoint.x - this->tl.x;
    tli.y = this->interPoint.y - this->tl.y;
    tli.z = this->interPoint.z - this->tl.z;

    //vector from the top left to the top right
    Point3D tlr;
    tlr.x = this->tr.x - this->tl.x;
    tlr.y = this->tr.y - this->tl.y;
    tlr.z = this->tr.z - this->tl.z;

    //square of the length of tlr
    double lenSqrTlr = tlr.x*tlr.x + tlr.y*tlr.y + tlr.z*tlr.z;

    //vector from the top left to the bottom left
    Point3D tlb;
    tlb.x = this->bl.x - this->tl.x;
    tlb.y = this->bl.y - this->tl.y;
    tlb.z = this->bl.z - this->tl.z;

    //square of the length of tlb
    double lenSqrTlb = tlb.x*tlb.x + tlb.y*tlb.y + tlb.z*tlb.z;

    Point3D coord2D;
    //dot product (tli and tlr)
    coord2D.x = tli.x*tlr.x + tli.y*tlr.y + tli.z*tlr.z;

    coord2D.x /= lenSqrTlr;

    //dot product (tli and tlb)
    coord2D.y = tli.x*tlb.x + tli.y*tlb.y + tli.z*tlb.z;

    coord2D.y /= lenSqrTlb;

    printf("Widget X: %.2f\n", coord2D.x);
    printf("Widget Y: %.2f\n", coord2D.y);

    return coord2D;
}

void Widget3D::startVideo()
{
    //not to start video several times
    if(!this->isVideoStarted)
    {
        this->isVideoStarted = true;
        //this->ffMpegReader = new FFmpegReader(QString("C:/My_Dev/vid/Yakon_mishelowki_1.avi"));

        QFile cfg("config.ini");
        bool openSuccess = cfg.open(QIODevice::ReadOnly | QIODevice::Text);

        if(openSuccess && cfg.isReadable())
        {
            QByteArray fileStr = cfg.readLine();
            fileStr.chop(1); //remove "\r\n"

           printf("Path to movie: %s\n", fileStr.data());

            this->ffMpegReader = new FFmpegReader(QString(fileStr.data()));//new FFmpegReader(QString("D:/Video/JM/JM.avi"));
            this->ffVideoThread = new FFVideoThread(this->ffMpegReader);
            this->ffVideoThread->start();
        }
    }
}

void Widget3D::stopVideo()
{
    if(this->ffVideoThread)
    {
        this->ffVideoThread->isPlaying = false;
        this->isVideoStarted = false;

        this->ffVideoThread->terminate();
        this->ffVideoThread->wait();

        delete this->ffVideoThread;
        this->ffVideoThread = NULL;
        

        if(this->ffMpegReader)
        {
            delete this->ffMpegReader;
            this->ffMpegReader = NULL;
        }
    }
}

Widget3D::~Widget3D()
{
    if(this->ffVideoThread)
        delete this->ffVideoThread;
    if(this->ffMpegReader)
        delete this->ffMpegReader;
}


