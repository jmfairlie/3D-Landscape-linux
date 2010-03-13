#ifndef NAMEFILTER_H
#define NAMEFILTER_H

#include "SearchFilter.h"

class NameFilter : public SearchFilter
{
public:
    NameFilter(QString name);

    QString nameToFilter;
};

#endif //NAMEFILTER_H
 
