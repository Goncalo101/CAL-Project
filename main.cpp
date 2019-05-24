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
            if (tag.find("building=warehouse") != string::npos || tag.find("industrial=warehouse") != string::npos ||
                    tag.find("landuse=industrial") != string::npos || tag.find("amenity=loading_dock") != string::npos) {
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
            if (tag.find("building=warehouse") != string::npos || tag.find("industrial=warehouse") != string::npos ||
                tag.find("landuse=industrial") != string::npos || tag.find("amenity=loading_dock") != string::npos) {
                initials.push_back(vertex->getInfo());
                break;
            }
        }
    }
}

GraphViewer* init_viewer(int width, int height)
{
    auto gv = new GraphViewer(width, height, false);
    gv->createWindow(width, height);
    gv->addNode(0, 0, 0);

    return gv;
}

int main(int argc, char* argv[])
{
    if (argc!=2) {
        cout << "Usage: " << argv[0] << " <city name>" << endl;
        exit(1);
    }

    string city_name(argv[1]);
    transform(city_name.begin(), city_name.end(), city_name.begin(), ::toupper);
    transform(city_name.begin()+1, city_name.end(), city_name.begin()+1, ::tolower);

    GraphViewer* gv = init_viewer(700, 600);


    Location loc1 = Location(402328904);
    Location loc2 = Location(402328897);

    Graph<Location> location_graph(city_name, gv);

    cout << location_graph.getNumVertex() << endl;

    /* DEFINE VERTEXES
     *
     * vector<Location*> initials = getPossibleInitialLocations(graph);
     *
     *      *DECIDE ON A INITIAL VERTEX
     *
     Vertex<Location> *initial_vertex = graph.findVertex(initial->getLocation().getID());

     vector<Location*> accessible_locations = graph.dfs(initial_vertex);

     vector<Location*> final_points = getPossibleFinalLocations(graph, accessible_locations);

            DECIDE ON A FINAL POINT(S)

     vector<DeliveryPoint*> deliveries = associateItems(items, graph, accessible_locations);
     */

    //this will probably be inside some funtion in the future
    location_graph.floydWarshallShortestPath();
    vector<Location> path = location_graph.getFloydWarshallPath(loc1, loc2);

    cout << "First try\n";
    for (int l = 0; l < path.size(); l++) {
        std::cout << path[l].getID() << endl;
    }

    if (!path.empty()) {
        gv->setVertexColor(path[0].getID(), "blue");
        for (int i = 1; i<path.size(); i++) {
            gv->setVertexColor(path[i].getID(), "green");
        }
        gv->setVertexColor(path[path.size()-1].getID(), "cyan");
    }

    getchar();
}
