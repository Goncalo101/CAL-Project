#ifndef CAL_PROJECT_ITEM_H
#define CAL_PROJECT_ITEM_H

#include "Location.h"

class Item {
private:
    int weight;
    int location_id;
    std::string destinatario;
    int value;
    int numFatura;
public:
    Item(int weight, int id,std::string destinatario,int value,int numFatura);
    int getWeight();
    int getLocation();
};

#endif //CAL_PROJECT_ITEM_H
