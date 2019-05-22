//
// Created by meias on 21-05-2019.
//

#ifndef CAL_PROJECT_ITEM_H
#define CAL_PROJECT_ITEM_H

#include "Location.h"

class Item {
private:
    int weight;
    Location location;
public:
    Item(int weight, Location& location);
    int getWeight();
    Location* getLocation();
};

#endif //CAL_PROJECT_ITEM_H
