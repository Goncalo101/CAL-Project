#include <iostream>
#include "Graph.h"
#include "Location.h"

int main()
{
    auto *gv = new GraphViewer(750, 600, false);
    gv->createWindow(750, 600);
    gv->addNode(0, 0, 0);

    Location loc1 = Location(402328881);
    Location loc2 = Location(402328911);

    Graph<Location> location_graph("Fafe", gv);

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

