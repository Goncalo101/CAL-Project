#include <iostream>
#include "Graph.h"
#include "Location.h"
#include "Item.h"
#include "InterestPoint.h"


void deleteUnusedVertexes(Graph<Location>& graph){
    vector<Vertex<Location>*> locations = graph.getVertexes();

    for(Vertex<Location> *vertex : locations){
        if(vertex->getInfo()->getType() == UNUSED){
            graph.removeVertex(*vertex->getInfo());
        }
    }
}


vector<Location*> getPossibleFinalLocations(vector<Location *> &accessible_locations){
    vector<Location*> finals;

    for (Location *location : accessible_locations) {
        for (string tag : location->getTags()) {
            if (tag.find("building=warehouse") != string::npos) {
                finals.push_back(location);
                break;
            }
        }
    }
}

vector<DeliveryPoint*> associateItems(vector<Item*>& items, vector<Location *> &accessible_locations) {
    vector<DeliveryPoint *> deliveries;
    bool shop;

    for (Item *item : items) {
        for (Location *location : accessible_locations) {
            if (item->getLocation()->getID() == location->getID()) {
                shop = false;

                for (string tag : location->getTags()) {
                    if (tag.find("shop") != string::npos) {
                        shop = true;
                        break;
                    }
                }

                if(shop){
                    continue;
                }

                switch (location->getType()) {
                    case UNUSED: {
                        DeliveryPoint *point = new DeliveryPoint(*location);

                        deliveries.push_back(point);

                        point->addItem(item);

                        location->set_type(point->getType());
                        break;
                    }
                    case DELIVER: {
                        for (DeliveryPoint *point : deliveries) {
                            if (point->getLocation().getID() == location->getID()) {
                                point->addItem(item);
                            }
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    }

    return deliveries;
}

vector<Location*> getPossibleInitialLocations(Graph<Location> & graph){
    vector<Location*> initials;

    for (Vertex<Location> *vertex : graph.getVertexes()) {
        for (string tag : vertex->getInfo()->getTags()) {
            if (tag.find("amenity=loading_dock") != string::npos) {
                initials.push_back(vertex->getInfo());
                break;
            }
        }
    }
}

int main()
{
    auto* gv = new GraphViewer(750, 600, false);
    gv->createWindow(750, 600);
    gv->addNode(0, 0, 0);

    Graph<Location> graph("Fafe", gv);

    cout << graph.getNumVertex() << endl;

    vector<Location*> initial_points = getPossibleInitialLocations(graph);

    Vertex<Location> *initial_vertex = graph.findVertex(initial_points[0]->getID());

    InitialPoint *initial = new InitialPoint(*initial_points[0]);

    vector<Location*> accessible_locations = graph.dfs(initial_vertex);

    vector<Location*> final_points = getPossibleFinalLocations(accessible_locations);

    FinalPoint *final = new FinalPoint(*final_points[0]);

    vector<DeliveryPoint*> deliveries = associateItems(items, accessible_locations);



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
