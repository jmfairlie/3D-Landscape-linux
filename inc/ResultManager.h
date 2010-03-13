#ifndef RESULTMANAGER_H
#define RESULTMANAGER_H

#include <QPainter>
#include <QList>
#include "Engine3D.h"
#include "ResultItem.h"

class Engine3D;

class ResultManager
{
public:
    ResultManager(Engine3D *engine3D);

    virtual ~ResultManager();

    void deleteItems();

    void addChild(QString label);

    void render(QPainter *painter);

    void resize(QSize mainWidgetSize);

    void processMouseClick(int x, int y);

    Engine3D *engine3D;

    QList<ResultItem*> resultItems;
};

#endif //RESULTMANAGER_H
