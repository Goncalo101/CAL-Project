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
#include "graphviewer/graphviewer.h"
#include "MutablePriorityQueue.h"

#define INF 10000000

template<class T>
class Edge;

template<class T>
class Graph;

template<class T>
class Vertex;

/****************** Provided structures  ********************/

template<class T>
class Vertex {
    T info;                // contents
    // list of outgoing edges
    bool visited;          // auxiliary field used by dfs and bfs
    int indegree;          // auxiliary field used by topsort
    bool processing;       // auxiliary field used by isDAG

    double dist = 0;
    Vertex<T>* path = NULL;
    int queueIndex = 0;        // required by MutablePriorityQueue

    void addEdge(Vertex<T>* dest, double w);

    bool removeEdgeTo(Vertex<T>* d);

public:
    explicit Vertex(T in);

    bool operator<(Vertex<T>& vertex); // // required by MutablePriorityQueue

    T* getInfo();

    friend class Graph<T>;
    std::vector<Edge<T>> adj;
    friend class MutablePriorityQueue<Vertex<T>>;
};

template<class T>
class Edge {
    Vertex<T>* dest;       // destination vertex
    // edge weight
public:
    Edge(Vertex<T>* d, double w);

    friend class Graph<T>;

    friend class Vertex<T>;
    double weight;
};

template<class T>
class Graph {
    std::vector<Vertex<T>*> vertexSet;    // vertex set
    void dfsVisit(Vertex<T>* v, std::vector<T*>& res) const;
    Vertex<T>* findVertex(const T& in) const;
    array<double, 2> extract_position(std::ifstream& lat_lon_file, std::ifstream& x_y_file);
    void extract_edges(std::ifstream& edges_file, GraphViewer* gv);
    void extract_tags(std::ifstream& tags_file);

    //used in Floyd-Warshall
    double** W = nullptr;
    double** P = nullptr;
    int findVertexIdx(const T& in) const;

public:
    Graph(const std::string& city_name, GraphViewer* gv);

    vector<Vertex<T>*> getVertexes() const;

    int getNumVertex() const;

    bool addVertex(const T& in);

    bool removeVertex(const T& in);

    bool addEdge(Vertex<T>* v1, Vertex<T>* v2, double w);

    bool removeEdge(const T& sourc, const T& dest);

    std::vector<T*> dfs(Vertex<T>* v) const;

    void floydWarshallShortestPath();

    vector<T> getFloydWarshallPath(const T& origin, const T& destination) const;

    ~Graph();

    Vertex<T>* findVertex(const int& id) const;
    void delete_inaccessible();
    vector<T> getPath(const T& origin, const T& dest) const;
    void dijkstraShortestPath(const T& origin);
    bool relax(Vertex<T>* u, Vertex<T>* v, double w);
    Vertex<T>* initSingleSource(const T& origin);
};

/****************** Provided constructors and functions ********************/

template<class T>
std::array<double, 2> Graph<T>::extract_position(std::ifstream& lat_lon_file, std::ifstream& x_y_file)
{

    if (!lat_lon_file.is_open() && !x_y_file.is_open()) return {};

    std::string line;
    std::getline(lat_lon_file, line);
    std::getline(x_y_file, line);

    int num_lines = std::stoi(line);

    std::array<char, 3> chars_to_remove = {',', '(', ')'};

    double max_x = 0, max_y = 0;
    for (int i = 0; i<num_lines; ++i) {
        double id, lat, lon, x, y;

        std::getline(lat_lon_file, line);
        for (char symbol : chars_to_remove) {
            line.erase(std::remove(line.begin(), line.end(), symbol), line.end());
        }

        std::istringstream lat_lon(line);
        lat_lon >> id >> lat >> lon;
        lat_lon.sync();
        lat_lon.clear();

        std::getline(x_y_file, line);
        for (char symbol : chars_to_remove) {
            line.erase(std::remove(line.begin(), line.end(), symbol), line.end());
        }

        std::istringstream x_y(line);
        x_y >> id >> x >> y;
        lat_lon.sync();
        lat_lon.clear();

        max_x = std::max(max_x, x);
        max_y = std::max(max_y, y);

        this->addVertex(T(x, y, lat, lon, id));
    }

    return {max_x, max_y};
}

template<class T>
void Graph<T>::extract_edges(std::ifstream& edges_file, GraphViewer* gv)
{
    if (!edges_file.is_open()) return;

    std::string line;

    std::getline(edges_file, line);
    int num_lines = std::stoi(line);

    std::array<char, 3> chars_to_remove = {',', '(', ')'};

    for (int i = 0; i<num_lines; ++i) {
        std::getline(edges_file, line);
        for (char symbol : chars_to_remove) {
            line.erase(std::remove(line.begin(), line.end(), symbol), line.end());
        }

        int source_id, dest_id;
        std::istringstream iss(line);
        iss >> source_id >> dest_id;
        iss.sync();
        iss.clear();

        Vertex<T>* source_vertex = this->findVertex(source_id);
        Vertex<T>* dest_vertex = this->findVertex(dest_id);
        double distance = sqrt(pow(source_vertex->info.getX()-dest_vertex->info.getX(), 2)+
                pow(source_vertex->info.getY()-dest_vertex->info.getY(), 2));
        this->addEdge(source_vertex, dest_vertex, distance);
        this->addEdge(dest_vertex, source_vertex, distance);
        gv->addEdge(i, source_vertex->info.getID(), dest_vertex->info.getID(), EdgeType::UNDIRECTED);
    }
}

template<class T>
void Graph<T>::extract_tags(std::ifstream& tags_file)
{
    if (!tags_file.is_open()) return;

    std::string line;

    std::getline(tags_file, line);
    int num_tags = stoi(line);

    for (int i = 0; i<num_tags; ++i) {
        std::getline(tags_file, line);

        std::string tags_to_add = line;

        std::getline(tags_file, line);
        int num_ids = stoi(line);

        for (int j = 0; j<num_ids; ++j) {
            std::getline(tags_file, line);
            int id = stoi(line);

            Vertex<T>* vertex = findVertex(id);

            if (vertex==nullptr) continue;

            vertex->info.add_tag(tags_to_add);
        }
    }
}

/**
 * Build graph from map files
 * @tparam T
 * @param city_name
 */
template<class T>
Graph<T>::Graph(const std::string& city_name, GraphViewer* gv)
{
    std::map<std::string, std::ifstream> input_files;
    std::array<std::string, 4> names = {"edges", "lat_lon", "x_y", "tags"};
    std::array<std::string, 4> file_prefixes = {"/T04_edges_", "/T04_nodes_lat_lon_", "/T04_nodes_X_Y_", "/T04_tags_"};

    for (int i = 0; i<4; ++i) {
        std::ostringstream file_name_stream;
        file_name_stream << "T04/" << city_name << file_prefixes[i] << city_name << ".txt";

        input_files.emplace(names[i], std::ifstream(file_name_stream.str()));
    }

    std::array<double, 2> pos_arr = extract_position(input_files["lat_lon"], input_files["x_y"]);

    for (Vertex<T>* vertex : vertexSet) {
        int id = vertex->info.getID();
        gv->addNode(id, vertex->info.getX()-pos_arr[0], vertex->info.getY()-pos_arr[1]);
        gv->setVertexLabel(id, to_string(id));
        gv->setVertexSize(id, 10);
        vertex->info.setX(vertex->info.getX()-pos_arr[0]);
        vertex->info.setY(vertex->info.getY()-pos_arr[1]);
    }

    extract_edges(input_files["edges"], gv);

    gv->rearrange();

    extract_tags(input_files["tags"]);
    std::cout << "Graph done" << std::endl;
}

template<class T>
Vertex<T>::Vertex(T in)
        : info(in) { }

template<class T>
Edge<T>::Edge(Vertex<T>* d, double w)
        : dest(d), weight(w) { }

template<class T>
int Graph<T>::getNumVertex() const
{
    return vertexSet.size();
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
template<class T>
Vertex<T>* Graph<T>::findVertex(const int& id) const
{
    for (auto v : vertexSet)
        if (v->info.getID()==id)
            return v;
    return nullptr;
}

/*
 *  Adds a vertex with a given content/info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template<class T>
bool Graph<T>::addVertex(const T& in)
{
    if (findVertex(in)!=nullptr)
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
bool Graph<T>::addEdge(Vertex<T>* v1, Vertex<T>* v2, double w)
{
    if (v1==nullptr || v2==nullptr)
        return false;
    v1->addEdge(v2, w);
    return true;
}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template<class T>
void Vertex<T>::addEdge(Vertex<T>* d, double w)
{
    adj.push_back(Edge<T>(d, w));
}

/*
 * Removes an edge from a graph (this).
 * The edge is identified by the source (sourc) and destination (dest) contents.
 * Returns true if successful, and false if such edge does not exist.
 */
template<class T>
bool Graph<T>::removeEdge(const T& sourc, const T& dest)
{
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1==nullptr || v2==nullptr)
        return false;
    return v1->removeEdgeTo(v2);
}

/*
 * Auxiliary function to remove an outgoing edge (with a given destination (d))
 * from a vertex (this).
 * Returns true if successful, and false if such edge does not exist.
 */
template<class T>
bool Vertex<T>::removeEdgeTo(Vertex<T>* d)
{
    for (auto it = adj.begin(); it!=adj.end(); it++)
        if (it->dest==d) {
            adj.erase(it);
            return true;
        }
    return false;
}

template<class T>
T* Vertex<T>::getInfo()
{
    return &info;
}
template<class T>
bool Vertex<T>::operator<(Vertex<T>& vertex)
{
    return getInfo()<vertex.getInfo();
}

/*
 *  Removes a vertex with a given content (in) from a graph (this), and
 *  all outgoing and incoming edges.
 *  Returns true if successful, and false if such vertex does not exist.
 */
template<class T>
bool Graph<T>::removeVertex(const T& in)
{
    for (auto it = vertexSet.begin(); it!=vertexSet.end(); it++)
        if ((*it)->info==in) {
            auto v = *it;
            vertexSet.erase(it);
            for (auto u : vertexSet) {
                u->removeEdgeTo(v);
                v->removeEdgeTo(u);
            }
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
std::vector<T*> Graph<T>::dfs(Vertex<T>* v) const
{
    std::vector<T*> res;

    for (auto vertex : vertexSet)
        vertex->visited = false;

    if (!v->visited)
        dfsVisit(v, res);
    return res;
}

/*
 * Auxiliary function that visits a vertex (v) and its adjacent not yet visited, recursively.
 * Updates a parameter with the list of visited node contents.
 */
template<class T>
void Graph<T>::dfsVisit(Vertex<T>* v, std::vector<T*>& res) const
{
    v->visited = true;
    res.push_back(&(v->info));
    for (auto& e : v->adj) {
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
        if (v->info==in)
            return v;
    return nullptr;
}

template<class T>
vector<Vertex<T>*> Graph<T>::getVertexes() const
{
    return vertexSet;
}

// ====== Floyd Warshall Algorithm =======
/*
 *Finds the index of a vertex with a given content
 */
template<class T>
int Graph<T>::findVertexIdx(const T& in) const
{
    for (unsigned int i = 0; i<vertexSet.size(); i++) {
        if (vertexSet[i]->info==in) {
            return i;
        }
    }
    return -1;
}

/*
 * Deletes a matrix
 */
template<class T>
void deleteMatrix(T** m, int n)
{
    if (m!=nullptr) {
        for (int i = 0; i<n; i++) {
            if (m[i]!=nullptr) {
                delete[] m[i];
            }
        }
        delete[] m;
    }
}

/*
 * Graph destructor
 */
template<class T>
Graph<T>::~Graph()
{
    deleteMatrix(W, vertexSet.size());
    deleteMatrix(P, vertexSet.size());
}

/*
 * The algorithm itself
 */
template<class T>
void Graph<T>::floydWarshallShortestPath()
{
    std::cout << "floyd-warshall started\n";
    unsigned n = vertexSet.size();
    deleteMatrix(W, n);
    deleteMatrix(P, n);

    W = new double* [n];
    P = new double* [n];

    for (unsigned int i = 0; i<n; i++) {
        W[i] = new double[n];
        P[i] = new double[n];
    }

    for (unsigned int i = 0; i<n; i++) {
        for (unsigned int j = 0; j<n; j++) {
            W[i][j] = i==j ? 0 : INF;
            P[i][j] = i==j ? i : -1;

        }
        for (auto e: vertexSet[i]->adj) {
            int j = findVertexIdx(e.dest->info);
            W[i][j] = e.weight;
            P[i][j] = i;
        }
    }

    for (unsigned int k = 0; k<n; k++) {
        for (unsigned int i = 0; i<n; i++) {
            for (unsigned int j = 0; j<n; j++) {

//                if (W[i][k]==INF || W[k][j]==INF) {
//                    continue;
//                }

                int val = W[i][k]+W[k][j];
                if (val<W[i][j]) {
                    W[i][j] = val;
                    P[i][j] = P[k][j];
                }
            }
        }
    }

    std::cout << "floyd-warshall done\n";
}

template<class T>
void Graph<T>::delete_inaccessible()
{
    for (auto vertex : vertexSet) {
        if (!vertex->visited)
            this->removeVertex(vertex->info);
    }
}

template<class T>
vector<T> Graph<T>::getFloydWarshallPath(const T& origin, const T& destination) const
{
    vector<T> res;
    int i = findVertexIdx(origin);
    int j = findVertexIdx(destination);

    for (int k = 0; k<vertexSet.size(); ++k) {
        for (int l = 0; l<vertexSet.size(); ++l) {
            std::cout << W[k][l] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << i << j << std::endl;

    if (i==-1 || j==-1 || W[i][j]==INF) {
        std::cout << "Path not found\n";
        return res;
    }
    for (; j!=i; j = P[i][j]) {
        res.push_back(vertexSet[j]->info);
    }

    reverse(res.begin(), res.end());

    std::cout << "Path calculated\n";
    return res;
}

template<class T>
Vertex<T>* Graph<T>::initSingleSource(const T& origin)
{
    for (Vertex<T>* vertex : this->vertexSet) {
        vertex->visited = false;
        vertex->dist = INF;
        vertex->path = NULL;
    }

    Vertex<T>* origin_vertex = findVertex(origin);
    if (origin_vertex == NULL) return NULL;

    origin_vertex->dist = 0;
    return origin_vertex;
}

template<class T>
bool Graph<T>::relax(Vertex<T>* u, Vertex<T>* v, double w)
{
    if (v->dist>u->dist+w) {
        v->dist = u->dist+w;
        v->path = u;
        return true;
    }

    return false;
}

template<class T>
void Graph<T>::dijkstraShortestPath(const T& origin)
{
    auto origin_vertex = initSingleSource(origin);

    if (origin_vertex == NULL) return;

    MutablePriorityQueue<Vertex<T>> queue;

    queue.insert(origin_vertex);

    while (!queue.empty()) {
        Vertex<T>* min_vertex = queue.extractMin();

        for (Edge<T> edge : min_vertex->adj) {
            if (relax(min_vertex, edge.dest, edge.weight)) {
                if (queue.find(edge.dest)) {
                    queue.decreaseKey(edge.dest);
                }
                else {
                    queue.insert(edge.dest);
                }
            }
        }
    }
}

template<class T>
vector<T> Graph<T>::getPath(const T& origin, const T& dest) const
{
    vector<T> res;

    Vertex<T>* dest_vertex = findVertex(dest);

    if (dest_vertex==nullptr || dest_vertex->dist==INF) return res;

    while (dest_vertex!=nullptr) {
        res.push_back(dest_vertex->info);
        dest_vertex = dest_vertex->path;
    }

    reverse(res.begin(), res.end());

    return res;
}

#endif //CAL_PROJECT_GRAPH_H

