//
// Created by goncalo on 20/05/19.
//

#include "Location.h"

Location::Location(int x, int y, int lat, int lon, int id)
{
    this->x = x;
    this->y = y;
    this->lat = lat;
    this->lon = lon;
    this->id = id;
}

Location::Location(int id) {
    this->id = id;
    this->x = -1;
    this->y = -1;
    this->lat = -1;
    this->lon = -1;
}

void Location::set_tags(std::string tags)
{
    this->tags = tags;
}

bool Location::operator==(const Location &l1)
{
    return this->id == l1.id;
}
