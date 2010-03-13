#ifndef ROUTEGRAPH_H
#define ROUTEGRAPH_H

#include "Utils3D.h"
#include <QList>

#include "routenode.h"
#include "routeedge.h"

#include <iostream>

using namespace std;

class RouteGraph
{
public:
    RouteGraph();

    QList<RouteNode *> nodes;

    void constructGraph();
    void renderGraph();

    virtual ~RouteGraph();
};

#endif // ROUTEGRAPH_H
