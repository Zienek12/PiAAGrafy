#include "graphs/adjacency_list_graph.hpp"
#include <algorithm>
#include <stdexcept>

//metody uaktualniajce
int AdjacencyListGraph::insertVertex(int val = 0)
{
    int id = nextVertexId++;
    vertices[id] = val;
    adjacencyList[id] = {};
    return id;
}

int AdjacencyListGraph::insertEdge(int v1, int v2, int weight)
{
    if(vertices.find(v1) == vertices.end() || vertices.find(v2) == vertices.end())
    {
        throw std::runtime_error("Vertex does not exist");
    }

    int edgeId = nextEdgeId++;
    edges[edgeId] = {v1, v2, weight};
    adjacencyList[v1].push_back(edgeId);
    return edgeId;
}

void AdjacencyListGraph::removeVertex(int v)
{
    if (vertices.find(v) == vertices.end())
    {
        throw std::runtime_error("Vertex does not exist");
    }

    //usuwanie wszystkich krawedzi przylegajacych

    for (int edgeId : adjacencyList.at(v))
    {
        //znajdowanie drugiego wierzcholka krawedzi
        const Edge& e = edges.at(edgeId);
        int neighbor = (e.v1 == v) ? e.v2 : e.v1;

        //usuwanie krawedzi z listy sasiedztwa sasiada
        auto& neigborEdges = adjacencyList.at(neighbor);


        //std::remove przekazuje wszystkie krawedzie edgeId na koniec wektora i zwraca iterator na ostani element nie wliczajac
        //wiec usuniety zostanie zakres zawierajacy tylko krawedzie ktorych szukamy
        neigborEdges.erase(std::remove(neigborEdges.begin(), neigborEdges.end(), edgeId), neigborEdges.end());


        edges.erase(edgeId);
    }
    vertices.erase(v);
    adjacencyList.erase(v);
    
}

void AdjacencyListGraph::removeEdge(int e)
{

    if (edges.find(e) == edges.end())
    {
        throw std::runtime_error("Edge does not exist");
    }

    const Edge& edge = edges.find(e)->second;
    int v1 = edge.v1;
    int v2 = edge.v2;

    auto removeFromAdjList = [](auto& adjList, int vertex, int edgeToRemove) { 
        auto& edges = adjList.at(vertex);
        edges.erase(std::remove(edges.begin(), edges.end(), edgeToRemove), edges.end());
        };
    removeFromAdjList(adjacencyList, v1, e);
    removeFromAdjList(adjacencyList, v2, e);
    edges.erase(e);


}




//metody iterujace
std::vector<int> AdjacencyListGraph::showVertices() const
{
    std::vector<int> result;
    for(const auto& [id, val] : vertices) // Iteracja po parach (klucz, wartość)
    {
        result.push_back(id); // Dodajemy tylko klucz (id) do wektora
    }
    return result;
}

std::vector<int> AdjacencyListGraph::showEdges() const
{
    std::vector<int> result;
    for(const auto& [id, _] : edges)
    {
        result.push_back(id);
    }
    return result;
}


std::vector<int> AdjacencyListGraph::incidentVertices(int v) const
{
    if(adjacencyList.find(v) == adjacencyList.end())
    {
        throw std::runtime_error("Vertex does not exist");
    }

    std::vector<int> result;

    for (int edgeId : adjacencyList.at(v))
    {
        const Edge& edge = edges.at(edgeId);
        result.push_back((edge.v1 == v) ? edge.v2 : v);
    }

    return result;
}



//Metody dostepu

std::vector<int> AdjacencyListGraph::endVertices(int edge) const
{
    if(edges.find(edge) == edges.end())
    {
        throw std::runtime_error("Edge does not exist");
    }
    const Edge& e = edges.at(edge);
    return {e.v1, e.v2};
}

bool AdjacencyListGraph::areAdjacent(int v1, int v2) const
{
    for (int edgeId : adjacencyList.at(v1))
    {
        const Edge& e = edges.at(edgeId);
        if(e.v1 == v2 || e.v2 == v1)
            return 1;
    }
    return 0;
}


int AdjacencyListGraph::opposite(int v, int e) const
{
    auto edgeId = edges.find(e);
    if (edgeId == edges.end())
    {
        throw std::runtime_error("Edge does not exist");
    }
    const Edge& edge = edgeId->second;
    if(edge.v1 == v)
        return edge.v1;
    else if(edge.v2 == v)
        return edge.v2;

    throw std::runtime_error("Vertex does not belong to the edge");
}


void AdjacencyListGraph::replaceVertices(int v, int val)
{
    auto vertexId = vertices.find(v);
    if (vertexId == vertices.end())
    {
        throw std::runtime_error("Vertex does not exist");
    }

    vertexId->second = val;
}

void AdjacencyListGraph::replaceEdges(int e, int weight)
{
    auto edgeId = edges.find(e);
    if (edgeId == edges.end())
    {
        throw std::runtime_error("Edge not existing");
    }

    edgeId->second.weight = weight;
}


std::unique_ptr<Graph> AdjacencyListGraph::createGraph(std::istream& is)
{
    auto graph = std::make_unique<AdjacencyListGraph>();
    int vertexCount, edgeCount;
    is >> vertexCount >> edgeCount;

    for(int i = 0; i < vertexCount; ++i)
    {
        graph->insertVertex();
    }

    for(int i = 0; i < edgeCount; ++i)
    {
        int v1, v2, weight;
        is >> v1 >> v2 >> weight;
        graph->insertEdge(v1, v2, weight);
    }

    return graph;
}




void AdjacencyListGraph::printGraph() const
{

     std::cout << "\n=== REPREZENTACJA GRAFU ===\n";

     std::cout << "Tak jak wczytano:\n";
 
      // Wypisz liczbę wierzchołków i krawędzi
     std::cout << vertices.size() << " " << edges.size() << std::endl;

      // Wypisz wszystkie krawędzie w formacie: v1 v2 waga
      for(const auto& [id, edge] : edges)
      {
             std::cout << edge.v1 << " " << edge.v2 << " " << edge.weight << std::endl;
      }
 

    // Wypisz wszystkie wierzchołki
    std::cout << "Wierzcholki: ";
     for(const auto& [id, val] : vertices)
     {
          std::cout << id << " ";
     }
   
    std::cout << "\n\n";

    // Wypisz wszystkie krawędzie
    std::cout << "Krawedzie:\n";
    for(const auto& [id, edge] : edges)
    {
        std::cout << "  " << edge.v1 << " --" << edge.weight << "--> " << edge.v2 << "\n";
    }

    // Wypisz listy sąsiedztwa
    std::cout << "\nListy sasiedztwa:\n";
    for(const auto& [vertex, edgeIds] : adjacencyList)
    {
        std::cout << "  " << vertex << ": ";
        for(int edgeId : edgeIds)
        {
            const Edge& e = edges.at(edgeId);
            int neighbor = (e.v1 == vertex) ? e.v2 : e.v1;
            std::cout << neighbor << "(" << e.weight << ") ";
        }
        std::cout << "\n";
    }
}