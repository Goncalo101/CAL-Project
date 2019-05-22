#ifndef CAL_PROJECT_LOCATION_H
#define CAL_PROJECT_LOCATION_H


#include <string>
#include <vector>

enum TYPE {GARAGE, DEPOT, DELIVER, UNUSED};

class Location
{
    int x, y, lat, lon, id;
    std::vector<std::string> tags;
    TYPE type;

public:
    Location(int x, int y, int lat, int lon, int id);

    void add_tag(std::string tag);

    void set_type(TYPE type);

    int getX() { return x; }

    int getY() { return y; }

    int getID() { return id; }

    std::vector<std::string> getTags();

    TYPE getType();

    bool operator==(const Location &l1);

    Location();
};


#endif //CAL_PROJECT_LOCATION_H
