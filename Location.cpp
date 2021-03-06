#include <cmath>
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

Location::Location() { }

void Location::add_tag(std::string tag)
{
    tags.push_back(tag);
}

void Location::set_type(TYPE type)
{
    this->type = type;
}

bool Location::operator==(const Location& l1)
{
    return this->id==l1.id;
}

Location::Location(int id) {
    this->id = id;
    this->x = -1;
    this->y = -1;
    this->lat = -1;
    this->lon = -1;
}

TYPE Location::getType()
{
    return type;
}

std::vector<std::string> Location::getTags()
{
    return tags;
}

bool Location::operator<(const Location &l1)
{
    return sqrt(pow(this->x, 2) + pow(this->y, 2)) < sqrt(pow(l1.x, 2) + pow(l1.y, 2)) ;
}

