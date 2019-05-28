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
    std::vector<Item*> items;
    std::vector<Location> path;

    Truck(int id, double capacity);
    int getID();
    double getCapacity();
    void addItem(Item * item);
    std::vector<Item*> getItems();
    void addPath(std::vector<Location> &path);
    std::vector<Location> getPath();
};


#endif //CAL_PROJECT_TRUCK_H
