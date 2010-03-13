#ifndef OPTIONSFILTER_H
#define OPTIONSFILTER_H

#include "SearchFilter.h"

class OptionsFilter : public SearchFilter
{
public:
    OptionsFilter(QString name);

    QList<QString> services;
};

#endif //OPTIONSFILTER_H
 
