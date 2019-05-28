#include <iostream>
#include <string>
#include "Graph.h"
#include "Location.h"
#include "Item.h"
#include "InterestPoint.h"

#define MAX_WEIGHT 50
#define MAX_VALUE  500
#define MAX_FATURA 1000000

static vector<int> usedLocationIds;

void compute_path(Graph<Location>& graph, const vector<DeliveryPoint*>& deliveries, vector<Location>& path,
        vector<DeliveryPoint*>& delivery_queue);
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

    for (Item* item : items) {
        for (Location* location : accessible_locations) {
            if (item->getLocation()==location->getID()) {
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

std::vector<Item*> itemFactory(int numItems,vector<Vertex<Location>*> vertexSet)  {
    vector<Item*> res;
    srand(time(NULL));

    vector<string> dest_first_name = {"Joaquim","João","Gonçalo","Leonardo","Sofia","Pedro","Ana","Mariana","Joana"};
    vector<string> dest_last_name = {"Morgado","Oliveira","Lajes","Moura","Pereira","Mendes","Baptista","Ferreira"};

    for (int i = 0; i < numItems; i++) {

        int locationIndex = rand() % vertexSet.size();
        int locationID = vertexSet[locationIndex]->getInfo()->getID();
        usedLocationIds.push_back(locationID);
        int weight = rand() % MAX_WEIGHT + 1;
        string dest = dest_first_name[rand() % dest_first_name.size()] + " " + dest_first_name[rand() % dest_first_name.size()] + " " + dest_last_name[rand() % dest_last_name.size()];

        Item* item = new Item(weight,locationID,dest,rand() % MAX_VALUE + 1, rand() % MAX_FATURA + 1);
        res.push_back(item);
    }

    return res;
}

void freeItems(std::vector<Item*> vec) {
    for (int i = 0; i < vec.size(); i++) {
        delete vec[i];
    }
}

void compute_path(Graph<Location>& graph, const vector<DeliveryPoint*>& deliveries, vector<Location>& path,
        vector<DeliveryPoint*>& delivery_queue)
{
    for (int i = 0; i<deliveries.size(); ++i) {
        vector<Location> temp;
        Location origin_location = deliveries[i]->getLocation();

        graph.dijkstraShortestPath(origin_location);

        Vertex<Location>* min_vertex = graph.getVertexes()[0];
        int min_idx = 0;
        for (int j = 0; j<delivery_queue.size(); ++j) {
            if (i==j) continue;

            Vertex<Location>* vertex = graph.findVertex(delivery_queue[j]->getLocation().getID());

            if (vertex==nullptr) continue;

            if (vertex->getDist()<min_vertex->getDist()) {
                min_vertex = vertex;
                min_idx = j;
            }
        }

        delivery_queue.erase(delivery_queue.begin()+min_idx);

        Location destination_location = *min_vertex->getInfo();

        temp = graph.getPath(origin_location, destination_location);
        path.insert(path.end(), temp.begin(), temp.end());
    }
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
    vector<Item*> items = itemFactory(5,graph.getVertexes());

    vector<DeliveryPoint*> deliveries = associateItems(items, accessible_locations);

    vector<Location> path;
    vector<DeliveryPoint*> delivery_queue(deliveries);

    graph.dijkstraShortestPath(initial->getLocation());

    Vertex<Location>* min_vertex = graph.getVertexes()[0];
    int min_idx = 0;
    for (int j = 0; j<delivery_queue.size(); ++j) {
        Vertex<Location>* vertex = graph.findVertex(delivery_queue[j]->getLocation().getID());

        if (vertex==nullptr) continue;

        if (vertex->getDist()<min_vertex->getDist()) {
            min_vertex = vertex;
            min_idx = j;
        }
    }

    delivery_queue.erase(delivery_queue.begin()+min_idx);

    vector<Location> path = graph.getFloydWarshallPath(Location(423840626), Location(423840615));
    path = graph.getPath(initial->getLocation(), *min_vertex->getInfo());

    /*
    for (auto vertex : graph.getVertexes()) {
        for (auto edge : vertex->adj)
            cout << edge.weight << endl;
    compute_path(graph, deliveries, path, delivery_queue);

    cout << "------------------\n";
    for (Location location : path) {
        cout << location.getID() << endl;
    }
     */

    cout << "First try\n";
    for (int l = 0; l < path.size(); l++) {
        std::cout << path[l].getID() << endl;
    }
    cout << "------------------\n";

    if (!path.empty()) {
        gv->setVertexColor(path[0].getID(), "blue");
        for (int i = 1; i<path.size(); i++) {
            gv->setVertexColor(path[i].getID(), "green");
        }
        gv->setVertexColor(path[path.size()-1].getID(), "red");
    }


    freeItems(items);

    getchar();
}