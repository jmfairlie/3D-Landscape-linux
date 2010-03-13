#ifndef POPUP_H
#define POPUP_H

#include <QPainter>
#include <QPointF>
#include <Utils3D.h>
#include <iostream>
#include <QMouseEvent>

using namespace std;

enum EPopupStatus
{
  E_PHIDDEN,
  E_PGROWING,
  E_PSHRINKING,
  E_PDISABLED,
  E_PENABLED
};


class Popup
{
    public:
    Popup();

    void show();
    void hide();
    void update();
    void render(QPainter* p,QPointF pos,QString dstName);
    void startGrowth();
    void startShrinkage();
    bool click(QMouseEvent* e);
    void grow();
    void shrink();
    void resize(float w, float h);

    QRect box;

    QFont font;

    QPointF size;
    QPointF tailRatio;
    float scale;
    EPopupStatus status;
    float growthFactor;
    float screenSizeRatio;
};
#endif // POPUP_H
