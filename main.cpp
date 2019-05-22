#include <iostream>
#include "Graph.h"
#include "Location.h"
#include "Item.h"
#include "InterestPoint.h"


vector<DeliveryPoint*> associateItems(vector<Item*> & items, Graph<Location> &graph){
    vector<Location*> accessible_locations = graph.dfs();
    vector<DeliveryPoint*> deliveries;

    for(Item* item : items){
        for(Location* location : accessible_locations){
            if(item->getLocation()->getID() == location->getID()){
                if(location->getType() == UNUSED){
                    DeliveryPoint* point = new DeliveryPoint(*location);

                    deliveries.push_back(point);

                    point->addItem(item);

                    location->set_type(point->getType());
                }
                else if(location->getType() == DELIVER){
                    for(DeliveryPoint* point : deliveries){
                        if(point->getLocation().getID() == location->getID()){
                            point->addItem(item);
                        }
                    }
                }
            }
        }
    }

    return deliveries;
}



int main()
{
    GraphViewer *gv = new GraphViewer(1350, 768, true);
    gv->createWindow(1350, 768);
    Graph<Location> graph("Fafe", gv);



    getchar();
}

