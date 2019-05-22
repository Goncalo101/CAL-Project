//
// Created by meias on 21-05-2019.
//

#include "Item.h"

Item::Item(int weight, Location& location)
        :weight(weight), location(location) { }

int Item::getWeight()
{
    return weight;
}

Location* Item::getLocation()
{
    return &location;
}
