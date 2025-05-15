#ifndef ADJACENCY_MATRIX_GRAPH_HPP_
#define ADJACENCY_MATRIX_GRAPH_HPP_

#include <memory>
#include <unordered_map>
#include "graphs/graph.hpp"
#include <climits>
class AdjacencyMatrixGraph : public Graph
{
    struct Edge{
        int v1;
        int v2;
        int weight;
    };


  private:
    std::vector<std::vector<int>> adjacencyMatrix;
    std::unordered_map<int, int> vertices;            
    std::unordered_map<int, Edge> edges; 
    int nextVertexIndex = 0;
    int nextEdgeIndex = 0;
    static int const INF = INT_MAX;
  public:
    // Update methods
    int insertVertex(int val) override;
    int insertEdge(int v1, int v2, int weight) override;
    void removeVertex(int v) override;
    void removeEdge(int e) override;

    // Iteration methods
    std::vector<int> showVertices() const override;
    std::vector<int> showEdges() const override;
    std::vector<int> incidentEdges(int v) const override;

    // Access methods
    std::vector<int> endVertices(int edge) const override;
    bool areAdjacent(int v1, int v2) const override;
    int opposite(int v, int e) const override;
    void replaceVertices(int v, int val) override;
    void replaceEdges(int e, int weight) override;

    void printGraph() const override;

    static std::unique_ptr<Graph> createGraph(std::istream& is);
};

#endif /* ADJACENCY_MATRIX_GRAPH_HPP_ */
