#include "SearchFilter.h"

TGlobalFilter::TGlobalFilter()
{
    this->name = "*";

    this->centreX = 0;
    this->centreZ = 0;
    this->radius = 0;
}

SearchFilter::SearchFilter(QString name)
{
    this->name = name;
}
