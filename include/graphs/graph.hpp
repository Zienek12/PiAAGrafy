#ifndef GRAPH_HPP_
#define GRAPH_HPP_

#include <iostream>
#include <memory>
#include <vector>

class Graph
{
  public:
    // TODO: implement all required methods

    virtual ~Graph() = default;

    // Metody uaktualniajace
    virtual int insertVertex(int val) = 0;
    virtual int insertEdge(int v1, int v2, int weight) = 0;
    virtual void removeVertex(int v) = 0;
    virtual void removeEdge(int e) = 0;

    // Metody iterujace
    virtual std::vector<int> showVertices() const = 0;
    virtual std::vector<int> showEdges() const = 0;
    virtual std::vector<int> incidentVertices(int v) const = 0;

    //Metody dostepu
    virtual std::vector<int> endVertices(int edge) const = 0;
    virtual bool areAdjacent(int v1, int v2) const = 0;
    virtual int opposite(int v, int e) const = 0;
    virtual void replaceVertices(int v, int val) = 0;
    virtual void replaceEdges(int e, int weight) = 0;

    virtual void printGraph() const = 0;
};

#endif /* GRAPH_HPP_ */
