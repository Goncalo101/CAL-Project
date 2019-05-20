#include <iostream>
#include "Graph.h"
#include "Location.h"

int main() {
    Graph<Location> location_graph("Fafe");
    location_graph.dfs();
}

