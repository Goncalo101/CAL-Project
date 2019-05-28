#include <iostream>
#include <string>
#include "Graph.h"
#include "Location.h"
#include "Item.h"
#include "InterestPoint.h"
#include "Truck.h"

#define MAX_WEIGHT 50
#define MAX_VALUE  500
#define MAX_FATURA 1000000
#define MAX_TRUCK_CAPACITY 500
#define MIN_TRUCK_CAPACITY 100
#define MAX_TRUCKS 3

static vector<int> usedLocationIds;

GraphViewer* init_viewer(int width, int height)
{
    auto gv = new GraphViewer(width, height, false);
    gv->createWindow(width, height);
    gv->addNode(0, 0, 0);

    return gv;
}

//return true if an id has been used
bool checkUsedId(int id)
{
    for (int i = 0; i<usedLocationIds.size(); i++) {
        if (id==usedLocationIds[i]) {
            return true;
        }
    }

    return false;
}

std::vector<Item*> itemFactory(int numItems, vector<Location> vertexSet)
{
    vector<Item*> res;

    vector<string> dest_first_name = {"Joaquim", "João", "Gonçalo", "Leonardo", "Sofia", "Pedro", "Ana", "Mariana",
                                      "Joana"};
    vector<string> dest_last_name = {"Morgado", "Oliveira", "Lajes", "Moura", "Pereira", "Mendes", "Baptista",
                                     "Ferreira"};

    for (int i = 0; i<numItems; i++) {

        int locationIndex = rand()%vertexSet.size();
        int locationID = vertexSet[locationIndex].getID();

        if (checkUsedId(locationID)) {
            continue;
        }

        usedLocationIds.push_back(locationID);
        int weight = rand()%MAX_WEIGHT+1;
        string dest =
                dest_first_name[rand()%dest_first_name.size()]+" "+dest_first_name[rand()%dest_first_name.size()]+" "
                        +dest_last_name[rand()%dest_last_name.size()];

        Item* item = new Item(weight, locationID, dest, rand()%MAX_VALUE+1, rand()%MAX_FATURA+1);
        res.push_back(item);
    }

    return res;
}

void freeItems(std::vector<Item*> vec)
{
    for (int i = 0; i<vec.size(); i++) {
        delete vec[i];
    }
}

void compute_path(Graph<Location>& graph, const vector<DeliveryPoint*>& deliveries, vector<Location>& path,
        vector<DeliveryPoint*>& delivery_queue)
{
    Location origin_location = path.back();
    vector<Location> path_temp;

    while (!delivery_queue.empty())  {
        graph.dijkstraShortestPath(origin_location);

        Vertex<Location>* min_vertex = new Vertex<Location>(Location(0));
        min_vertex->setDist(INF);
        int min_idx = 0;
        for (int j = 0; j < delivery_queue.size(); ++j) {
            if (delivery_queue[j]->getLocation() == origin_location) continue;

            Vertex<Location>* vertex = graph.findVertex(delivery_queue[j]->getLocation().getID());

            if (vertex == nullptr) continue;

            if (vertex->getDist() < min_vertex->getDist()) {
                min_vertex = vertex;
                min_idx = j;
            }
        }

        delivery_queue.erase(delivery_queue.begin() + min_idx);
        path_temp = graph.getPath(origin_location, *min_vertex->getInfo());
        path.insert(path.end(), path_temp.begin(), path_temp.end());

        origin_location = path_temp.back();
        path_temp.clear();
    }
}

vector<Vertex<Location>> get_garages(const Graph<Location>& graph)
{
    vector<Vertex<Location>> garages;

    for (int i = 0; i < 10; ++i) {
        int index = rand() % graph.getVertexes().size();
        bool id_used = false;

        for (int location_id : usedLocationIds) {
            if (location_id == index) {
                id_used = true;
            }
        }

        if (id_used) {
            --i;
        } else {
            garages.push_back(*graph.getVertexes()[rand() % graph.getVertexes().size()]);
        }
    }

    return garages;
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

    srand(time(nullptr));

    // load graphviewer
    GraphViewer* gv = init_viewer(700, 600);

    // load graph
    Graph<Location> graph(city_name, gv);

    // pick an initial point
    Vertex<Location> initial_vertex = *graph.getVertexes()[rand()%graph.getVertexes().size()];

    // perform dfs starting in the initial vertex chosen earlier and delete vertices that are inaccessible from those
    vector<Location> accessible_locations = graph.dfs(&initial_vertex);
    graph.delete_inaccessible();

    // pick a final point
    Vertex<Location> final_point = *graph.getVertexes()[rand()%graph.getVertexes().size()];
    vector<Vertex<Location>> garages = get_garages(graph);

    // generate random items
    vector<Item*> items = itemFactory(5, accessible_locations);

    vector<DeliveryPoint*> deliveries;

    for (Item* item : items) {
        deliveries.push_back(new DeliveryPoint(Location(item->getLocation())));
    }

    //initialize truck array
    Truck* trucks[MAX_TRUCKS];
    for (int i = 0; i < MAX_TRUCKS; i++) {
        Truck* truck = new Truck((rand() % (MAX_TRUCK_CAPACITY - MIN_TRUCK_CAPACITY)) + MAX_TRUCK_CAPACITY);
        trucks[i] = truck;
    }

    vector<Location> path;
    vector<DeliveryPoint*> delivery_queue(deliveries);

    graph.dijkstraShortestPath(*initial_vertex.getInfo());

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

    if (!delivery_queue.empty()) {
        delivery_queue.erase(delivery_queue.begin()+min_idx);

        path = graph.getPath(*initial_vertex.getInfo(), *min_vertex->getInfo());

        compute_path(graph, deliveries, path, delivery_queue);

        graph.dijkstraShortestPath(path.back());

        min_vertex = graph.findVertex(garages[0].getInfo()->getID());
        for (Vertex<Location> vertex : garages) {
            if (vertex.getDist() < min_vertex->getDist()) {
                min_vertex = &vertex;
            }
        }

        vector<Location> path_temp = graph.getPath(path.back(), *min_vertex->getInfo());
        path.insert(path.end(), path_temp.begin(), path_temp.end());
    }

    cout << "------------------\n";
    for (Location& l : path) {
        std::cout << l.getID() << endl;
    }
    cout << "------------------\n";

    if (!path.empty()) {
        gv->setVertexColor(path[0].getID(), "blue");
        gv->setVertexSize(path[0].getID(), 20);
        for (int i = 1; i<path.size(); i++) {
            gv->setVertexColor(path[i].getID(), "green");
            gv->setVertexSize(path[i].getID(), 20);
        }
        for (DeliveryPoint* delivery_point : deliveries) {
            gv->setVertexColor(delivery_point->getLocation().getID(), "black");
            gv->setVertexSize(delivery_point->getLocation().getID(), 20);
        }
        gv->setVertexSize(path.back().getID(), 20);

        for (Vertex<Location> vertex : garages) {
            gv->setVertexColor(vertex.getInfo()->getID(), "pink");
        }

    }

    freeItems(items);

    getchar();
}
