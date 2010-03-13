#include "PricesFilter.h"

PricesFilter::PricesFilter(QString name)
        : SearchFilter(name)
{
    this->type = E_PRICESFILTER;
}
