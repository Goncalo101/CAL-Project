#include <iostream>
#include "Graph.h"
#include "Location.h"

int main()
{
    auto *gv = new GraphViewer(750, 600, false);
    gv->createWindow(750, 600);
    gv->addNode(0, 0, 0);

    Graph<Location> location_graph("Maia", gv);

    getchar();
}

