#include "Item.h"

Item::Item(int weight, int id, std::string dest, int value, int numFatura)
        :weight(weight), location_id(id), destinatario(dest), value(value), numFatura(numFatura) { }

int Item::getWeight()
{
    return weight;
}

int Item::getLocation()
{
    return location_id;
}
