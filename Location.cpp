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
    this->type = UNUSED;
}

Location::Location() {}

void Location::add_tag(std::string tag)
{
    tags.push_back(tag);
}

void Location::set_type(TYPE type) {
    this->type = type;
}

bool Location::operator==(const Location &l1)
{
    return this->id == l1.id;
}

TYPE Location::getType() {
    return type;
}

std::vector<std::string> Location::getTags() {
    return tags;
}

