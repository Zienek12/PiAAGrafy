#include "graphs/adjacency_matrix_graph.hpp"
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <iomanip>


int AdjacencyMatrixGraph::insertVertex(int val = 0)
{
    int newIndex = nextVertexIndex++;
    vertices[newIndex] = val;

    //Dodanie nowego wiersza wypelnionego INF
    adjacencyMatrix.push_back(std::vector<int>(newIndex, INF));


    //Rozszerzenie istniejacych wierszy
    for (auto& row : adjacencyMatrix)
    {
        row.push_back(INF);
    }
    return newIndex;
}


int AdjacencyMatrixGraph::insertEdge(int v1, int v2, int weight)
{

    int newEdgeIndex = nextEdgeIndex++;
    adjacencyMatrix[v1][v2] = weight;

    edges[newEdgeIndex] = {v1, v2, weight};
    return newEdgeIndex;
}


void AdjacencyMatrixGraph::removeVertex(int v)
{
    for (auto edgeIt = edges.begin(); edgeIt != edges.end();)
    {
        Edge& edge = edgeIt->second;
        if (edge.v1 == v || edge.v2 == v)
        {
            adjacencyMatrix[edge.v1][edge.v2] = INF;
            adjacencyMatrix[edge.v2][edge.v1] = INF;
            edgeIt = edges.erase(edgeIt);

        }
        else
        {
            ++edgeIt;
        }
    }


    adjacencyMatrix.erase(adjacencyMatrix.begin() + v);
    for (auto& row : adjacencyMatrix)
    {
        row.erase(row.begin() + v);
    }
    vertices.erase(v);

}


void AdjacencyMatrixGraph::removeEdge(int e) 
{
    Edge& edge = edges[e];
    adjacencyMatrix[edge.v1][edge.v2] = INF;
    edges.erase(e);
   
}


std::vector<int> AdjacencyMatrixGraph::showVertices() const
{
    std::vector<int> result;
    for (const auto& [idx, val] : vertices)
    {
        result.push_back(idx);
    }
    return result;
}

std::vector<int> AdjacencyMatrixGraph::showEdges() const
{
    std::vector<int> result;
    for (const auto& [idx, _] : edges)
    {
        result.push_back(idx);
    }
    return result;
}


std::vector<int> AdjacencyMatrixGraph::incidentEdges(int v) const
{
    std::vector<int> result;
    for(const auto& [edgeId, edge] : edges)
    {
        if(edge.v1 == v || edge.v2 == v)
            result.push_back(edgeId);
    }
    return result;
}

std::vector<int> AdjacencyMatrixGraph::endVertices(int edge) const
{
    const Edge& e = edges.at(edge);

    return {e.v1, e.v2};
}


bool AdjacencyMatrixGraph::areAdjacent(int v1, int v2) const
{
    return adjacencyMatrix[v1][v2] != INF;
}

int AdjacencyMatrixGraph::opposite(int v, int edge) const
{
    const Edge& e = edges.at(edge);
    if(e.v1 == v)
        return e.v2;
    if(e.v2 == v)
        return e.v1;
    throw std::invalid_argument("Wierzcholek nie jest czescia krawedzi");
}

void AdjacencyMatrixGraph::replaceVertices(int v, int val)
{
    vertices[v] = val;
}

void AdjacencyMatrixGraph::replaceEdges(int e, int weight)
{
    Edge& edge = edges[e];
    adjacencyMatrix[edge.v1][edge.v2] = weight;
    edges[e].weight = weight;
    
}

void AdjacencyMatrixGraph::printGraph() const
{
    // Nagłówek
    std::cout << "Macierz sąsiedztwa:\n";
    std::cout << "     ";
    for(int j = 0; j < nextVertexIndex; ++j)
    {
        std::cout << std::setw(1) << j << " ";
    }
    std::cout << "\n";

    // Separator
    std::cout << "   +";
    for(int j = 0; j < nextVertexIndex; ++j)
    {
        std::cout << "----";
    }
    std::cout << "\n";

    // Zawartość
    for(int i = 0; i < nextVertexIndex; ++i)
    {
        std::cout << std::setw(2) << i << " |";
        for(int j = 0; j < nextVertexIndex; ++j)
        {
            if(adjacencyMatrix[i][j] == INF)
            {
                std::cout << std::setw(3) << u8"\u221E" << " ";
            }
            else
            {
                std::cout << std::setw(3) << adjacencyMatrix[i][j] << " ";
            }
        }
        std::cout << "\n";
    }

    std::cout << "Legenda krawedzi";
    for (const auto& [id, edge] : edges)
    {
        std::cout << "\nId " << id << ": " << edge.v1 << " --" << edge.weight << "--> " << edge.v2;
    }
    std::cout << "\n";

}


std::unique_ptr<Graph> AdjacencyMatrixGraph::createGraph(std::istream& is)
{
    int vertexCount, edgesCount;
    is >> vertexCount >> edgesCount;
    auto graph = std::make_unique<AdjacencyMatrixGraph>();

    for (int i = 0; i < vertexCount; i++)
    {
        graph->insertVertex();
    }

    for (int i = 0; i < edgesCount; i++)
    {
        int v1, v2, weight;
        is >> v1 >> v2 >> weight;
        graph->insertEdge(v1, v2, weight);
    }



    return graph;
}
