#include <Engine3D.h>
#include <qapplication.h>

#include <QGraphicsScene>
#include <QGraphicsView>


class GraphicsView : public QGraphicsView
{
public:
    GraphicsView()
    {
        setWindowTitle(tr("Tampere 3D"));
    }

protected:
    void resizeEvent(QResizeEvent *event) {
        if (scene())
            scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
        QGraphicsView::resizeEvent(event);
    }
};

int main( int argc, char **argv )
{
    QApplication a( argc, argv );

    //Engine3D *w = new Engine3D();

    //w->show();
    Engine3D w;
    w.show();

    return a.exec();
}

/*


bool MouseActive;
int MouseRotX ,MouseRotY;





void processMouse(int button, int state, int x, int y)
{
        if (state == GLUT_DOWN)
        {
                MouseRotX = x;
                MouseRotY = y;
                MouseActive = true;
        }
        else if (state == GLUT_UP)
        {
                MouseActive = false;
        }
}


void processMouseActiveMotion(int x, int y)
{
 if (MouseActive)
   {
                                int dX, dY;
                                dX = x - MouseRotX;
                                dY = MouseRotY - y;
                                myEngine->updateCameraRotation(dX,dY);

                                MouseRotX = x;
                                MouseRotY = y;
   }
}

int main(int argc, char** argv)
{

        MouseActive = false;
        glutInit(&argc, argv);
        glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //glut_depth doesnt seem to be necessary
  glutInitWindowSize (900, 600);
  glutInitWindowPosition (100, 100);
  glutCreateWindow ("3DEngine");

  glutDisplayFunc(render);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyHandler);
  glutMouseFunc(processMouse);
        glutMotionFunc(processMouseActiveMotion);
  myEngine = new Engine3D();
        Timer(0);
        glutMainLoop();
        return 0;
}


*/





