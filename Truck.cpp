//
// Created by meias on 28-05-2019.
//

#include "Truck.h"

Truck::Truck(double capacity) : id(id), capacity(capacity){
    static int tempId = 0;
    this->id = tempId += 1;
}

int Truck::getID() {
    return id;
}

double Truck::getCapacity() {
    return capacity;
}

void Truck::addItem(Item *item) {
    items.push_back(item);
    this->current_weight += item->getWeight();
}

std::vector<Item *> Truck::getItems() {
    return items;
}

void Truck::addPath(std::vector<Location> &path) {
    this->path = path;
}

std::vector<Location> Truck::getPath() {
    return path;
}

void Truck::addPathLoc(Location &location) {
    this->path.push_back(location);
}




