//
// Created by meias on 28-05-2019.
//

#ifndef CAL_PROJECT_TRUCK_H
#define CAL_PROJECT_TRUCK_H

#include <vector>
#include "Item.h"

class Truck {
    int id;
    double capacity;
    double current_weight = 0;
    std::vector<Item*> items;
    std::vector<Location> path;

public:
    Truck(double capacity);
    int getID();
    double getCapacity();
    void addItem(Item * item);
    std::vector<Item*> getItems();
    void addPath(std::vector<Location> &path);
    void addPathLoc(Location &location);
    std::vector<Location> getPath();
};


#endif //CAL_PROJECT_TRUCK_H
