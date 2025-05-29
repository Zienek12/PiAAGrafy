#include "graphs/adjacency_list_graph.hpp"
#include <algorithm>
#include <stdexcept>

//metody uaktualniajce
//  Dodaje nowy wierzchołek o wartości val (domyślnie 0).
//  Tworzy nowy identyfikator, dodaje do mapy vertices i tworzy pustą listę sąsiedztwa.
//  Złożoność czasowa: O(1), pamięciowa: O(1)
int AdjacencyListGraph::insertVertex(int val = 0)
{
    int id = nextVertexId++;
    vertices[id] = val;
    adjacencyList[id] = {};
    return id;
}

// Dodaje nową krawędź między v1 i v2 o wadze weight.
// Sprawdza istnienie wierzchołków, tworzy nowy identyfikator krawędzi, dodaje do mapy edges
// i do listy sąsiedztwa v1.
// Złożoność czasowa: O(1), pamięciowa: O(1)
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



// Usuwa wierzchołek v oraz wszystkie incydentne krawędzie.
// Najpierw znajduje wszystkie krawędzie incydentne do v (przeglądając całą mapę edges),
// następnie usuwa je z mapy edges oraz z list sąsiedztwa sąsiadów.
// Na końcu usuwa v z mapy vertices i adjacencyList.
// Złożoność czasowa: O(E), gdzie E to liczba krawędzi (przeglądanie wszystkich krawędzi).
// Pamięciowa: O(1)
void AdjacencyListGraph::removeVertex(int v)
{
    if(v>= nextVertexId)
    {
        throw std::out_of_range("Vertex does not exist");
    }

    std::vector<int> edgesToRemove;

    edgesToRemove = incidentEdges(v);

    for(int edgeId : edgesToRemove)
    {
        const Edge& edge = edges.at(edgeId);

        int neighbor = (edge.v1 == v) ? edge.v2 : edge.v1;

        if(adjacencyList.find(neighbor) != adjacencyList.end())
        {
            auto& neighborEdges = adjacencyList.at(neighbor);
            neighborEdges.erase(std::remove(neighborEdges.begin(), neighborEdges.end(), edgeId), neighborEdges.end());
        }

        edges.erase(edgeId);
    }

    vertices.erase(v);
    adjacencyList.erase(v);
}

// Usuwa krawędź o identyfikatorze e.
// Sprawdza istnienie krawędzi, usuwa ją z mapy edges oraz z list sąsiedztwa obu końców krawędzi.
// Złożoność czasowa: O(d1 + d2), gdzie d1 i d2 to stopnie końców krawędzi (usuwanie z wektorów).
// Pamięciowa: O(1)
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


// metody iterujace

// Zwraca wektor identyfikatorów wszystkich wierzchołków w grafie.
// Przechodzi po mapie vertices i zbiera klucze.
// Złożoność czasowa: O(V), pamięciowa: O(V)
std::vector<int> AdjacencyListGraph::showVertices() const
{
    std::vector<int> result;
    for(const auto& [id, val] : vertices) 
    {
        result.push_back(id); 
    }
    return result;
}

// Zwraca wektor identyfikatorów wszystkich krawędzi w grafie.
// Przechodzi po mapie edges i zbiera klucze.
// Złożoność czasowa: O(E), pamięciowa: O(E)
std::vector<int> AdjacencyListGraph::showEdges() const
{
    std::vector<int> result;
    for(const auto& [id, _] : edges)
    {
        result.push_back(id);
    }
    return result;
}


// Zwraca wektor identyfikatorów krawędzi incydentnych do wierzchołka v.
// Przechodzi po wszystkich krawędziach i sprawdza, czy v jest końcem krawędzi.
// Złożoność czasowa: O(E), pamięciowa: O(d), gdzie d to liczba incydentnych krawędzi.

std::vector<int> AdjacencyListGraph::incidentEdges(int v) const
{
    if(adjacencyList.find(v) == adjacencyList.end())
    {
        throw std::runtime_error("Vertex does not exist");
    }

    std::vector<int> result;

    for(const auto& [edgeId, edge] : edges)
    {
        if(edge.v1 == v || edge.v2 == v)
            result.push_back(edgeId);
    }

    return result;
}



//Metody dostepu

// Zwraca parę wierzchołków końcowych krawędzi o podanym identyfikatorze.
// Złożoność czasowa: O(1), pamięciowa: O(1)

std::vector<int> AdjacencyListGraph::endVertices(int edge) const
{
    if(edges.find(edge) == edges.end())
    {
        throw std::runtime_error("Edge does not exist");
    }
    const Edge& e = edges.at(edge);
    return {e.v1, e.v2};
}

// Sprawdza, czy wierzchołki v1 i v2 są połączone krawędzią.
// Przechodzi po wszystkich krawędziach incydentnych do v1 i sprawdza, czy v2 jest drugim końcem.
// Złożoność czasowa: O(d1), gdzie d1 to liczba krawędzi incydentnych do v1.
// Pamięciowa: O(1)
bool AdjacencyListGraph::areAdjacent(int v1, int v2) const
{
    for (int edgeId : adjacencyList.at(v1))
    {
        const Edge& e = edges.at(edgeId);
        if(e.v1 == v2 || e.v2 == v2)
            return 1;
    }
    return 0;
}

// Zwraca wierzchołek przeciwny do v na krawędzi e.
// Jeśli v nie jest końcem krawędzi e, rzuca wyjątek.
// Złożoność czasowa: O(1), pamięciowa: O(1)
int AdjacencyListGraph::opposite(int v, int e) const
{
    auto edgeId = edges.find(e);
    if (edgeId == edges.end())
    {
        throw std::runtime_error("Edge does not exist");
    }
    const Edge& edge = edgeId->second;
    if(edge.v1 == v)
        return edge.v2;
    else if(edge.v2 == v)
        return edge.v1;

    throw std::runtime_error("Vertex does not belong to the edge");
}


// Zmienia wartość wierzchołka v na val.
// Złożoność czasowa: O(1), pamięciowa: O(1)
void AdjacencyListGraph::replaceVertices(int v, int val)
{
    auto vertexId = vertices.find(v);
    if (vertexId == vertices.end())
    {
        throw std::runtime_error("Vertex does not exist");
    }

    vertexId->second = val;
}

// Zmienia wagę krawędzi e na weight.
// Złożoność czasowa: O(1), pamięciowa: O(1)
void AdjacencyListGraph::replaceEdges(int e, int weight)
{
    auto edgeId = edges.find(e);
    if (edgeId == edges.end())
    {
        throw std::runtime_error("Edge not existing");
    }

    edgeId->second.weight = weight;
}


// Tworzy graf na podstawie danych wejściowych ze strumienia is.
// Najpierw wczytuje liczbę wierzchołków i krawędzi, następnie dodaje wierzchołki i krawędzie.
// Złożoność czasowa: O(V + E), pamięciowa: O(V + E)

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
         std::cout << id << "("<< val << ") ";
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
            std::cout << neighbor << "(" << e.weight << ", "<< edgeId << ") ";
        }
        std::cout << "\n";
    }
}