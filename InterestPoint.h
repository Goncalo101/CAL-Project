//
// Created by meias on 21-05-2019.
//

#ifndef CAL_PROJECT_INTERESTPOINT_H
#define CAL_PROJECT_INTERESTPOINT_H

#include "Location.h"
#include "Item.h"
#include <vector>

using namespace std;

class InterestPoint {
private:
    Location location;
protected:
    TYPE type;
public:
    InterestPoint();
    InterestPoint(Location location);
    virtual Location getLocation();
    virtual TYPE getType();
};

class InitialPoint: public InterestPoint {
public:
    InitialPoint();
    InitialPoint(Location location);
};

class FinalPoint: public InterestPoint {
public:
    FinalPoint();
    FinalPoint(Location location);
};

class DeliveryPoint: public InterestPoint {
    vector<Item*> items;
public:
    DeliveryPoint();
    DeliveryPoint(Location location);
    void addItem(Item* item);
    void addItems(vector<Item*> items);
    vector<Item*> getItems();
};

#endif //CAL_PROJECT_INTERESTPOINT_H
