#ifndef SEARCHFILTER_H
#define SEARCHFILTER_H

#include <QString>
#include <QList>

enum EFilterType
{
    E_NAMEFILTER = 5000,
    E_DISTANCEFILTER,
    E_OPTIONSFILTER,
    E_PRICESFILTER
};

//global filter, constructed from list of filters
struct TGlobalFilter
{
    TGlobalFilter();
    //name
    QString name;

    //services
    QList<QString> services;

    //distance
    int centreX;
    int centreZ;
    int radius;
};

class SearchFilter
{
public:
    SearchFilter(QString name);

    QString name;
    EFilterType type;
};

#endif //SEARCHFILTER_H
