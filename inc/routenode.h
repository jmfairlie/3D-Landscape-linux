#ifndef ROUTENODE_H
#define ROUTENODE_H

#include "Utils3D.h"
#include <QList>

#include "routeedge.h"

class RouteEdge;

class RouteNode
{
public:
    RouteNode(VECTOR4D coords, QString name);
    RouteNode();

    virtual ~RouteNode();

    QList<RouteEdge *> incidentEdges;
    QString name;

public: //temporary public
    VECTOR4D coords;
};

#endif //ROUTENODE_H
