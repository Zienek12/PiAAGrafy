#ifndef ADJACENCY_LIST_GRAPH_HPP_
#define ADJACENCY_LIST_GRAPH_HPP_

#include <memory>
#include <unordered_map>
#include <vector>
#include "graphs/graph.hpp"

class AdjacencyListGraph : public Graph
{
  private:
    struct Edge
    {
        int v1, v2;
        int weight;
    };

    std::unordered_map<int, int> vertices;//indeks wierzcholka i wartosc
    std::unordered_map<int, Edge> edges;//inedks krawedzi, polaczenie waga
    std::unordered_map<int, std::vector<int>> adjacencyList;

    int nextVertexId = 0;
    int nextEdgeId = 0;


  public:
    // TODO: implement all required methods
     //metody uaktualniajace
    int insertVertex(int val) override;
    int insertEdge(int v1, int v2, int weight) override;
    void removeVertex(int v) override;
    void removeEdge(int e) override;

    //metody iterujace
    std::vector<int> incidentEdges(int v) const override;
    std::vector<int> showVertices() const override;
    std::vector<int> showEdges() const override;
  

    //metody dostepu
    std::vector<int> endVertices(int edge) const override;
    int opposite(int v, int e) const override;
    bool areAdjacent(int v1, int v2) const override;
    void replaceVertices(int v, int val) override;
    void replaceEdges(int e, int weight) override;


    static std::unique_ptr<Graph> createGraph(std::istream& is);

    void printGraph() const override;



};

#endif /* ADJACENCY_LIST_GRAPH_HPP_ */
