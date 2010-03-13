#include "routenode.h"


RouteNode::RouteNode(VECTOR4D coords, QString name)
{
    this->coords = coords;
    this->name = name;
}

RouteNode::RouteNode(){}

RouteNode::~RouteNode()
{
    for(int i = 0; i < this->incidentEdges.size(); i++)
    {
        delete this->incidentEdges[i];
    }
}

