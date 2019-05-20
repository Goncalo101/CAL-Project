#ifndef CAL_PROJECT_LOCATION_H
#define CAL_PROJECT_LOCATION_H


#include <string>

class Location
{
    int x, y, lat, lon, id;
    std::string tags;

public:
    Location(int x, int y, int lat, int lon, int id);

    void set_tags(std::string tags);

    int getX() { return x; }

    int getY() { return y; }

    int getID() { return id; }

    bool operator==(const Location &l1);
};


#endif //CAL_PROJECT_LOCATION_H
