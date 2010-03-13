#ifndef _ENGINE3D_H
#define _ENGINE3D_H

#include <QApplication>
#include <QLineEdit>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QFormLayout>
#include <QDirModel>
#include <QTreeView>
#include <QTextEdit>
#include <Utils3D.h>
#include <Object3D.h>
#include <ColladaReader.h>
#include <TextureLoader.h>
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <qgl.h>
#include <qtimer.h>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QBtooth.h>
#include <QBthread.h>
#include <QMetaType>
#include <QWheelEvent>
#include <QPainter>
#include "Widget3D.h"
#include <cmath>
#include <ctime>

#include "MenuManager.h"

#include "imagewidget.h"
#include "routegraph.h"

#include "ResultManager.h"


using namespace std;
#include "DistanceSelector.h"
#include "OptionsSelector.h"
#include "PricesSelector.h"
#include "NameSelector.h"
#include "FilterSelector.h"
#include "SearchResSelector.h"

#include "DestinationBar.h"

#include "NameFilter.h"
#include "DistanceFilter.h"
#include "PricesFilter.h"
#include "OptionsFilter.h"
#include "Popup.h"

#include <QSound>

class ResultManager;

enum EAppState
{
    E_3DVIEW = 2000,
    E_FADE_IN,
    E_FADE_OUT,
    E_SELECTOR,
    E_LOADING_IV,
    E_ASCENDING,
    E_DESCENDING,
    E_NOTHING
};

enum ECurrSelector
{
    E_FILTER = 3000,
    E_DISTANCE,
    E_OPTIONS,
    E_PRICES,
    E_NAME,
    E_SR
};

struct TInsideObjectInfo
{
    QString info;
    VECTOR4D tailCoords;
};

class Engine3D : public QGLWidget
{

Q_OBJECT

public:
        Engine3D(QWidget *parent=0, char *name=0);
        ~Engine3D();
         void moveCameraRelative(GLfloat x, GLfloat y, GLfloat z);
         void saveCameraTransform();
         void restoreCameraPosition();
         void updateCameraRotation(int x, int y);

         void reshape(int w, int h);
         void zoomSideBar(float z);

        QList<Destination*> destinations;
        QList<SearchFilter*> searchFilters;
        Destination* currDestination;

        QList<Destination*> foundDestinations;

        GLuint selectBuf[SELBUFSIZE];
        void PickSelected(int x, int y);
        void processHits (GLint hits, GLuint buffer[]);

        bool checkCollisions(VECTOR4D p);

        void updateWidgetTexture();
        void updateVideoWidgetTexture();

        void createWidgets3D();
        void destroyWidgets3D();
        void populateSearchFilters();

        QPointF getScreenCoords(VECTOR4D worldCoords);

        void createDestItem(string name, string nodeName,
                              double x, double y, double z,
                              QString auxObjName="Can");

        void startDescensionToDestination(VECTOR4D destinationPos,
                                            VECTOR4D nodePos);

        void setCamera();
        void setSearchCamera();

        ECurrSelector selector;

protected:
        void initializeGL();
        void resizeGL( int width, int height );

        void paintEvent(QPaintEvent *event);

        void keyPressEvent(QKeyEvent *e );
        void view3DkeyboardProcess( QKeyEvent *e );
        void mousePressEvent(QMouseEvent *e );
        void mouseReleaseEvent(QMouseEvent *e );
        void mouseMoveEvent(QMouseEvent *e);
        void mouseDoubleClickEvent(QMouseEvent *e);
        void processSelectorMouse(QMouseEvent *e);
        void wheelEvent(QWheelEvent *e);

        void timeOut();

protected slots:
        void timeOutSlot();
        void textureTimerSlot();

        //Shake SK7 slots
        void printAccData(AccData accData);
        void isDisconnected(bool disconnected);

        void menuItemClicked(QString menuItemName);        

public: //temporary public
	//methods
        bool loadScene(string scenePath, string auxPath);
	bool load3DObjects();
	bool loadTextures();
	bool loadLights();
        void illuminate();

        VECTOR4D getWorldPoint(int x, int y);

        void updateSelector(QPainter *painter);

        void drawMapMarks(QPainter *painter);

        void resizeSelector(QSize size);

        //returns number of found destinations
        int destinationsSearch();

        //=true if screenshot is needed
        bool isMakeScreenShot;

        //if video widget is playing
        bool isVideoPlaying;

        //=true, if the user is in the inside view
        bool inInsideView;



	void setSideBarCamera();
	void drawAxes(float l, float w, float col);
	void rotateRelativeCamera(GLfloat x, GLfloat y, GLfloat z);
        void updateCameraPosition();
	void drawCoordRef(int x, int y , int z, float posx, float posy, float l);
	void DrawSideBar();
	void DrawMainView();
        void DrawAuxObjects();
        void DrawRouteGraph();
        void DrawView3D();
        void DrawInsideView();
        void DrawInsideView3D();        
        void DrawSearchView();
        void DrawSearchView3D();
        void DrawAscending();
        void DrawAscending3D();        
        void DrawRectangle(int red, int green, int blue, int alpha, QPainter &painter);
        void DrawCaca(/*QPainter &painter*/);
        int  getAuxObjIndexByName(string nodeName);
        int  getObjIndexByName(string nodeName);
        void createDestinations();
	void DrawTopBar();
        void drawSomeShit(QPainter *painter);
        void setUpSky();
        void drawSky();
        VECTOR4D getBezierValue(VECTOR4D origin, VECTOR4D control1,VECTOR4D destination, VECTOR4D control2,float t);
        void startDescentionToDestination(VECTOR4D destinationPos,
                                            VECTOR4D nodePos);
        void startAscension();
        void startDescension();
        void setTranslationToMatrix(Matrix4D m, VECTOR4D translation);

        QTimer *m_timer;
        QTimer *texture_timer;


        void destroySceneObjects();

        QList<Widget3D*> widgets3D;
        QList<Widget3D*> videoWidgets3D;

        EAppState applicationState;

        //=true, if function was called from mouse press event
        bool ProcessMouseEvent(QMouseEvent * e);
        void updateRay(int x, int y);

        int selectedWidget;
        int selectedVideoWidget;

        bool getVerticesAndImageIndex(QString nodeID, Point3D &tl, Point3D &bl,
            Point3D &tr, Point3D &br, GLuint &imageIndex);
        void getNodeTransform(int nodeIndex, Matrix4D transform);
        void renderMenu(QPainter* painter);
        void updateMenu();
        void resetCameraTransform();
        void translateCamera(GLfloat x, GLfloat y, GLfloat z);
        void printMatrix(int x, int y, Matrix4D m);
        void resetCameraRotations();

        void jumpToDestination(VECTOR4D destinationPos, VECTOR4D nodePos);

        //make screenshot of the current frame
        void makeScreenShot();
        void drawFrustrum();
        void updateFrustrumCoords();
        void reset3DProjectionMatrix();
        void commonKeyEvent(QKeyEvent *e);
        void turnLightsOn(bool on);

	//attributes

        VECTOR4D savedCameraPosition;
        VECTOR4D savedCameraRotation;

        VECTOR4D cameraPosition;
	VECTOR4D cameraRotation;
	float zoom;
        float zoomSearchView;
        //type of the destinations to search
        EType searchDestinationType;
	
        bool MouseActive;
        int MouseRotX;
        int MouseRotY;
	int screenw;
	int screenh;
	Object3D** objects;
        QList<Object3D> auxObjects;
        int fadeSpeed;
        QImage screenShot;

		
        DistanceSelector *distanceSelector;
        OptionsSelector *optionsSelector;
        PricesSelector *pricesSelector;
        NameSelector *nameSelector;
        FilterSelector *filterSelector;
        SearchResSelector *srSelector;

        glTexture* textures;
        glTexture skytexture;
        glTexture *auxTextures;
        float skyangle;

        Matrix4D  rot_matrix, inv_transform, currpos_matrix, proj_matrix;

	int* lights;
	int numObjects;
	int numTextures;
        int numAuxTextures;
	TextureLoader* textureLoader;
	ColladaReader * colladaLoader;
        ColladaReader * colladaAuxObjects;
	TGeomInfo geomInfo;
        MenuManager * menuManager;

        //information of the object hit in the inside view
        //QString info not equal to "" if this object contains information
        //to show in the popup
        //also contains coordinates of the popup tail
        TInsideObjectInfo currObjInfo;

        ResultManager *resultManager;

        QWidget *widgetUnder;

        RouteGraph *routeGraph;

        //alpha level for fade-in/fade-out
        int fadeAlphaLevel;

        //frustum params
        GLfloat fleft;
        GLfloat fright;
        GLfloat fbottom;
        GLfloat ftop;
        GLfloat fnear;
        GLfloat ffar;
        float collisionPadding;

        double nearPosX;
        double nearPosY;
        double nearPosZ;

        double farPosX;
        double farPosY;
        double farPosZ;
        float maxPitch; //max angle of rotation around x axis (look up/down)
        float currentPitch;
        GLUquadric* cameraLocator;
        GLUquadric* sky;
        //Shake SK7 connectivity
        QBtooth *bt;
        QBthread *bThr;
        int pathCounter;
        int pathLength;

        VECTOR4D pathOrigin;
        VECTOR4D pathControl1;
        VECTOR4D pathControl4;
        VECTOR4D pathDestination;
        VECTOR4D pathLookup;
        unsigned int frames;
        DWORD startTime;
        float fps;
        bool lightsOn;
        int numlights;
        float speed;


        GLdouble frustrumCoords[24];
        ImageWidget *iw;
        ImageWidget *iw3;
        Popup* popup;
        float frustumIndex;
};

#endif
