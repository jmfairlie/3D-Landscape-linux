#ifndef ROUTEEDGE_H
#define ROUTEEDGE_H

#include "routenode.h"

class RouteNode;

class RouteEdge
{
public:
    RouteEdge(RouteNode *firstEP, RouteNode *secondEP);
    RouteEdge();
public: //temporary public
    RouteNode *firstEP;
    RouteNode *secondEP;
};

#endif //ROUTEEDGE_H
