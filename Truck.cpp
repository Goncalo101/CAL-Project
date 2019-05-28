//
// Created by meias on 28-05-2019.
//

#include "Truck.h"

Truck::Truck(int id, double capacity) : id(id), capacity(capacity){}

int Truck::getID() {
    return id;
}

double Truck::getCapacity() {
    return capacity;
}

void Truck::addItem(Item *item) {
    items.push_back(item);
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




