#include <iostream>
#include "Graph.h"
#include "Location.h"

int main()
{
    GraphViewer *gv = new GraphViewer(1350, 768, true);
    gv->createWindow(1350, 768);
    Graph<Location> location_graph("Fafe", gv);

    getchar();
}

