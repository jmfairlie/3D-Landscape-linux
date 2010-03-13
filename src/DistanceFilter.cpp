#include "DistanceFilter.h"

DistanceFilter::DistanceFilter(QString name)
        : SearchFilter(name)
{
    this->type = E_DISTANCEFILTER;
    this->radius = 0;
}

void DistanceFilter::setRadius()
{
    this->radius = (int)sqrt((this->edgeX - this->centreX) *
                 (this->edgeX - this->centreX) +
                 (this->edgeZ - this->centreZ) *
                 (this->edgeZ - this->centreZ));
}


