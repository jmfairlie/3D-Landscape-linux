#ifndef DESTINATION_H
#define DESTINATION_H

#include "Utils3D.h"
#include <iostream>

using namespace std;

enum EType
{
  E_BAR = 1000,
  E_CINEMA
};

class Destination
{
public:
    Destination();

    string name;
    //name of the node in the model
    string nodeName;
    EType type;
    int auxObjIndex;
    bool highLighted;
    int hilightIndex;
    VECTOR4D coords;
    VECTOR4D nodeCoords;
};

#endif //DESTINATION_H
