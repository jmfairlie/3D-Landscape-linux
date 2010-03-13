#include "routeedge.h"

RouteEdge::RouteEdge(RouteNode *firstEP, RouteNode *secondEP)
{
    this->firstEP = firstEP;
    this->secondEP = secondEP;
}

RouteEdge::RouteEdge(){}
