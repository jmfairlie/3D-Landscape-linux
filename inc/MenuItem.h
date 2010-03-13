#ifndef MENUITEM_H

#define MENUITEM_H

#include <cmath>
#include <QPoint>
#include <QPainter>
#include <iostream>
#include "MenuManager.h"

using namespace std;

#define PI 3.14159265
enum EMenuItemStatus
{
  E_HIDDEN,
  E_GROWING,
  E_SHRINKING,
  E_EXPANDING,
  E_COLLAPSING,
  E_MOVING,
  E_DISABLED,
  E_ENABLED
};

class MenuManager;

class MenuItem
{
   public:
    MenuItem(QString name, QImage * icon, MenuItem * parent, QPointF size, MenuManager * manager);
    ~MenuItem();
    void grow();
    void shrink();
    void expand();
    void collapse();
    bool childReady();
    void render(QPainter *painter);
    void goToPosition(QPointF destination);
    bool hitTest(QPoint p);
    bool click(QPointF p);
    bool hover(QPointF p);
    void initgrow(QPointF startpos);
    void initShrinkage();
    void initExpansion();
    void initCollapse();
    void move();
    void update();
    void enable();
    void disable();
    void hide();
    void resetScale();
    QPointF getPos();
     int getStatus();
    void addChild(MenuItem* child);
    bool isInAncestorQueue();
    void initTranslation(MenuItem * initiator);
    void setChildOrder(int order);
    int getNumChildren();
    void initGoBackTranslation();
    void setInitiator(MenuItem* initiator);
    void resize(QPointF size);
    void resetItem();
    void emitClickEvent(QString name);
    void hideImmediate();
    void showImmediate(QPointF startpos);

    float speed;
    QPointF velocity;
    QPointF position;
    QPointF destination;
    MenuItem * parent;
    QPointF scale;
    QPointF size;
    QString name;
    QImage* icon;
    float growthRate;
    float expansionRadius;
    bool expanded;
    bool inAncestorQueue;
    bool isCurrent;
    int childOrder;
    QFont font;

    EMenuItemStatus status;
    int childReadyCount;
    int numChildren;
    MenuItem ** children;
    MenuItem * moveInitiator;
    MenuManager * manager;
    bool mouseOver;
};

#endif // MENUITEM_H
