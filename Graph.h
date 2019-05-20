#ifndef CAL_PROJECT_GRAPH_H
#define CAL_PROJECT_GRAPH_H

#include <vector>
#include <sstream>
#include <array>
#include <fstream>
#include <map>
#include <algorithm>
#include <iostream>
#include <cmath>

#include "graphviewer.h"

template<class T>
class Edge;

template<class T>
class Graph;

template<class T>
class Vertex;

/****************** Provided structures  ********************/

template<class T>
class Vertex
{
    T info;                // contents
    std::vector<Edge<T>> adj;  // list of outgoing edges
    bool visited;          // auxiliary field used by dfs and bfs
    int indegree;          // auxiliary field used by topsort
    bool processing;       // auxiliary field used by isDAG

    void addEdge(Vertex<T> *dest, double w);

    bool removeEdgeTo(Vertex<T> *d);

public:
    Vertex(T in);

    friend class Graph<T>;
};

template<class T>
class Edge
{
    Vertex<T> *dest;       // destination vertex
    double weight;         // edge weight
public:
    Edge(Vertex<T> *d, double w);

    friend class Graph<T>;

    friend class Vertex<T>;
};

template<class T>
class Graph
{
    std::vector<Vertex<T> *> vertexSet;    // vertex set


    void dfsVisit(Vertex<T> *v, std::vector<T> &res) const;

    Vertex<T> *findVertex(const int &id) const;

    Vertex<T> *findVertex(const T& in) const;

public:
    Graph(std::string city_name);

    int getNumVertex() const;

    bool addVertex(const T &in);

    bool removeVertex(const T &in);

    bool addEdge(Vertex<T> *v1, Vertex<T> *v2, double w);

    bool removeEdge(const T &sourc, const T &dest);

    std::vector<T> dfs() const;
};

/****************** Provided constructors and functions ********************/
/**
 * Build graph from map files
 * @tparam T
 * @param city_name
 */
template<class T>
Graph<T>::Graph(std::string city_name)
{
    std::map<std::string, std::ifstream> input_files;
    std::array<std::string, 4> names = {"edges", "lat_lon", "x_y", "tags"};
    std::array<std::string, 4> file_prefixes = {"/T04_edges_", "/T04_nodes_lat_lon_", "/T04_nodes_X_Y_", "/T04_tags_"};

    for (int i = 0; i < 4; ++i) {
        std::ostringstream file_name_stream;
        file_name_stream << "T04/" << city_name << file_prefixes[i] << city_name << ".txt";

        input_files.emplace(names[i], std::ifstream(file_name_stream.str()));
    }

    std::string line;
    std::getline(input_files["lat_lon"], line);
    std::getline(input_files["x_y"], line);

    int num_lines = std::stoi(line);

    std::array<char, 3> chars_to_remove = {',', '(', ')'};

    for (int i = 0; i < num_lines; ++i) {
        int id, lat, lon, x, y;

        std::getline(input_files["lat_lon"], line);
        for (char symbol : chars_to_remove) {
            line.erase(std::remove(line.begin(), line.end(), symbol), line.end());
        }

        std::istringstream lat_lon(line);
        lat_lon >> id >> lat >> lon;

        std::getline(input_files["x_y"], line);
        for (char symbol : chars_to_remove) {
            line.erase(std::remove(line.begin(), line.end(), symbol), line.end());
        }

        std::istringstream x_y(line);
        x_y >> id >> x >> y;

        this->addVertex(T(x, y, lat, lon, id));
    }

    std::getline(input_files["edges"], line);
    num_lines = std::stoi(line);

    for (int i = 0; i < num_lines; ++i) {
        std::getline(input_files["edges"], line);
        for (char symbol : chars_to_remove) {
            line.erase(std::remove(line.begin(), line.end(), symbol), line.end());
        }

        int source_id, dest_id;
        std::istringstream iss(line);
        iss >> source_id >> dest_id;

        Vertex<T> *source_vertex = this->findVertex(source_id);
        Vertex<T> *dest_vertex = this->findVertex(dest_id);
        double distance = sqrt(pow(source_vertex->info.getX() - dest_vertex->info.getX(), 2) +
                               pow(source_vertex->info.getY() - dest_vertex->info.getY(), 2));
        this->addEdge(source_vertex, dest_vertex, distance);
    }
}

template<class T>
Vertex<T>::Vertex(T in)
        : info(in)
{}

template<class T>
Edge<T>::Edge(Vertex<T> *d, double w)
        : dest(d), weight(w)
{}

template<class T>
int Graph<T>::getNumVertex() const
{
    return vertexSet.size();
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
template<class T>
Vertex<T> *Graph<T>::findVertex(const int &id) const
{
    for (auto v : vertexSet)
        if (v->info.getID() == id)
            return v;
    return nullptr;
}

/*
 *  Adds a vertex with a given content/info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template<class T>
bool Graph<T>::addVertex(const T &in)
{
    if (findVertex(in) != nullptr)
        return false;
    vertexSet.push_back(new Vertex<T>(in));
    return true;
}

/*
 * Adds an edge to a graph (this), given the contents of the source (sourc) and
 * destination (dest) vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template<class T>
bool Graph<T>::addEdge(Vertex<T> *v1, Vertex<T> *v2, double w)
{
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addEdge(v2, w);
    return true;
}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template<class T>
void Vertex<T>::addEdge(Vertex<T> *d, double w)
{
    adj.push_back(Edge<T>(d, w));
}

/*
 * Removes an edge from a graph (this).
 * The edge is identified by the source (sourc) and destination (dest) contents.
 * Returns true if successful, and false if such edge does not exist.
 */
template<class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest)
{
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    return v1->removeEdgeTo(v2);
}

/*
 * Auxiliary function to remove an outgoing edge (with a given destination (d))
 * from a vertex (this).
 * Returns true if successful, and false if such edge does not exist.
 */
template<class T>
bool Vertex<T>::removeEdgeTo(Vertex<T> *d)
{
    for (auto it = adj.begin(); it != adj.end(); it++)
        if (it->dest == d) {
            adj.erase(it);
            return true;
        }
    return false;
}

/*
 *  Removes a vertex with a given content (in) from a graph (this), and
 *  all outgoing and incoming edges.
 *  Returns true if successful, and false if such vertex does not exist.
 */
template<class T>
bool Graph<T>::removeVertex(const T &in)
{
    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++)
        if ((*it)->info == in) {
            auto v = *it;
            vertexSet.erase(it);
            for (auto u : vertexSet)
                u->removeEdgeTo(v);
            delete v;
            return true;
        }
    return false;
}

/*
 * Performs a depth-first search (dfs) in a graph (this).
 * Returns a vector with the contents of the vertices by dfs order.
 * Follows the algorithm described in theoretical classes.
 */
template<class T>
std::vector<T> Graph<T>::dfs() const
{
    std::vector<T> res;
    for (auto v : vertexSet)
        v->visited = false;
    for (auto v : vertexSet)
        if (!v->visited)
            dfsVisit(v, res);
    return res;
}

/*
 * Auxiliary function that visits a vertex (v) and its adjacent not yet visited, recursively.
 * Updates a parameter with the list of visited node contents.
 */
template<class T>
void Graph<T>::dfsVisit(Vertex<T> *v, std::vector<T> &res) const
{
    v->visited = true;
    res.push_back(v->info);
    for (auto &e : v->adj) {
        auto w = e.dest;
        if (!w->visited)
            dfsVisit(w, res);
    }
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
template<class T>
Vertex<T>* Graph<T>::findVertex(const T& in) const
{
    for (auto v : vertexSet)
        if (v->info == in)
            return v;
    return nullptr;
}

#endif //CAL_PROJECT_GRAPH_H
