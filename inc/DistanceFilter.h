#ifndef DISTANCEFILTER_H
#define DISTANCEFILTER_H
#include <cmath>

#include "SearchFilter.h"

class DistanceFilter : public SearchFilter
{
public:
    DistanceFilter(QString name);

    int centreX;
    int centreZ;
    int edgeX;
    int edgeZ;
    int radius;

    void setRadius();
};

#endif //DISTANCEFILTER_H
 
