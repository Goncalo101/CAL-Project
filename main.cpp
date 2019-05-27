#include <iostream>
#include "Graph.h"
#include "Location.h"
#include "Item.h"
#include "InterestPoint.h"

void deleteUnusedVertexes(Graph<Location>& graph)
{
    vector<Vertex<Location>*> locations = graph.getVertexes();

    for (Vertex<Location>* vertex : locations) {
        if (vertex->getInfo()->getType()==UNUSED) {
            graph.removeVertex(*vertex->getInfo());
        }
    }
}

vector<Location*> getPossibleFinalLocations(vector<Location*>& accessible_locations)
{
    vector<Location*> finals;

    for (Location* location : accessible_locations) {
        for (string tag : location->getTags()) {
            if (tag.find("building=warehouse")!=string::npos || tag.find("industrial=warehouse")!=string::npos ||
                    tag.find("landuse=industrial")!=string::npos || tag.find("amenity=loading_dock")!=string::npos) {
                finals.push_back(location);
                break;
            }
        }
    }

    return finals;
}

vector<DeliveryPoint*> associateItems(vector<Item*>& items, vector<Location*>& accessible_locations)
{
    vector<DeliveryPoint*> deliveries;
    bool shop;

    for (Item* item : items) {
        for (Location* location : accessible_locations) {
            if (item->getLocation()->getID()==location->getID()) {
                shop = false;

                for (string tag : location->getTags()) {
                    if (tag.find("shop")!=string::npos) {
                        shop = true;
                        break;
                    }
                }

                if (shop) {
                    continue;
                }

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

vector<Location*> getPossibleInitialLocations(Graph<Location>& graph)
{
    vector<Location*> initials;

    for (Vertex<Location>* vertex : graph.getVertexes()) {
        for (string tag : vertex->getInfo()->getTags()) {
            if (tag.find("building=warehouse")!=string::npos || tag.find("industrial=warehouse")!=string::npos ||
                    tag.find("landuse=industrial")!=string::npos || tag.find("amenity=loading_dock")!=string::npos) {
                initials.push_back(vertex->getInfo());
                break;
            }
        }
    }

    return initials;
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

    // sanitize input
    string city_name(argv[1]);
    transform(city_name.begin(), city_name.end(), city_name.begin(), ::toupper);
    transform(city_name.begin()+1, city_name.end(), city_name.begin()+1, ::tolower);

    // load graphviewer
    GraphViewer* gv = init_viewer(700, 600);

    // load graph
    Graph<Location> graph(city_name, gv);

    // compute valid locations for the initial point and pick one
    vector<Location*> initial_points = getPossibleInitialLocations(graph);
    Vertex<Location>* initial_vertex = graph.findVertex(initial_points[0]->getID());
    InitialPoint* initial = new InitialPoint(*initial_points[0]);

    // perform dfs starting in the initial vertex chosen earlier and delete vertices that are inaccessible from those
    vector<Location*> accessible_locations = graph.dfs(initial_vertex);
    graph.delete_inaccessible();

    // compute valid locations for the final points and pick one
    vector<Location*> final_points = getPossibleFinalLocations(accessible_locations);
//    FinalPoint* final = new FinalPoint(*final_points[1]);

    // TODO: get items from file or generate random items
    vector<Item*> items;

    vector<DeliveryPoint*> deliveries = associateItems(items, accessible_locations);

    // compute all pairs shortest path
    graph.floydWarshallShortestPath();

    vector<Location> path = graph.getFloydWarshallPath(Location(10), Location(13));

    for (auto vertex : graph.getVertexes()) {
        for (auto edge : vertex->adj)
            cout << edge.weight << endl;
    }

    if (!path.empty()) {
        gv->setVertexColor(path[0].getID(), "blue");
        for (int i = 1; i<path.size(); i++) {
            gv->setVertexColor(path[i].getID(), "green");
        }
        gv->setVertexColor(path[path.size()-1].getID(), "red");
    }

    getchar();
}
