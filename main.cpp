#include <iostream>
#include "Graph.h"
#include "Location.h"
#include "Item.h"
#include "InterestPoint.h"

vector<DeliveryPoint*> associateItems(vector<Item*>& items, Graph<Location>& graph)
{
    vector<Location*> accessible_locations = graph.dfs();
    vector<DeliveryPoint*> deliveries;

    for (Item* item : items) {
        for (Location* location : accessible_locations) {
            if (item->getLocation()->getID()==location->getID()) {
                switch (location->getType()) {
                case UNUSED: {
                    DeliveryPoint* point = new DeliveryPoint(*location);

                    deliveries.push_back(point);

                    point->addItem(item);

                    location->set_type(point->getType());
                    break;
                }
                case DELIVER: {
                    for (DeliveryPoint* point : deliveries) {
                        if (point->getLocation().getID()==location->getID()) {
                            point->addItem(item);
                        }
                    }
                    break;
                }

                default:break;
                }
            }
        }
    }

    return deliveries;
}

int main()
{
    auto* gv = new GraphViewer(750, 600, false);
    gv->createWindow(750, 600);
    gv->addNode(0, 0, 0);

    Location loc1 = Location(402328881);
    Location loc2 = Location(402328911);

    Graph<Location> location_graph("Fafe", gv);

    cout << location_graph.getNumVertex() << endl;

    //this will probably be inside some funtion in the future
    location_graph.floydWarshallShortestPath();
    vector<Location> path = location_graph.getFloydWarshallPath(loc1, loc2);

    if (path.size() == 0) {
    }
    else {
        gv->setVertexColor(path[0].getID(),"blue");
        for (int i = 1; i < path.size(); i++) {
            gv->setVertexColor(path[i].getID(),"green");
        }
        gv->setVertexColor(path[path.size()-1].getID(),"cyan");
    }

    getchar();
}
