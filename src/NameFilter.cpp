#include "NameFilter.h"

NameFilter::NameFilter(QString name)
        : SearchFilter(name)
{
    this->type = E_NAMEFILTER;
}
