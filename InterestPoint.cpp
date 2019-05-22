#include "InterestPoint.h"

InterestPoint::InterestPoint(Location location)
        :location(location) { }

Location InterestPoint::getLocation()
{
    return location;
}

TYPE InterestPoint::getType()
{
    return type;
}

InitialPoint::InitialPoint(Location location)
        :InterestPoint(location)
{
    type = DEPOT;
}

FinalPoint::FinalPoint(Location location)
        :InterestPoint(location)
{
    type = GARAGE;
}

DeliveryPoint::DeliveryPoint(Location location)
        :InterestPoint(location)
{
    type = DELIVER;
}

void DeliveryPoint::addItem(Item* item)
{
    items.push_back(item);
}

void DeliveryPoint::addItems(vector<Item*> items)
{
    for (Item* item : items) {
        this->items.push_back(item);
    }
}

vector<Item*> DeliveryPoint::getItems()
{
    return items;
}


