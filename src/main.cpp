#include "graphs/adjacency_list_graph.hpp"
#include "graphs/adjacency_matrix_graph.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <algorithm>
#include <windows.h>

void generateRandomGraph(const std::string& filename, int vertexCount, int edgeCount)
{
    if(vertexCount <= 0)
    {
        throw std::invalid_argument("Liczba wierzcholkow musi byc dodatnia");
    }
    if(edgeCount < 0 || edgeCount > vertexCount * (vertexCount - 1) / 2)
    {
        throw std::invalid_argument("Nieprawidlowa liczba krawedzi");
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> weightDist(1, 100);

    std::ofstream outFile(filename);
    if(!outFile)
    {
        throw std::runtime_error("Nie można otworzyc pliku do zapisu");
    }

    outFile << vertexCount << " " << edgeCount << "\n";

    std::vector<std::pair<int, int>> possibleEdges;
    //zewnetrzna petla oznacza indeks wierzcholka z ktorego wychodzi krawedz
    for(int i = 0; i < vertexCount; ++i)
    {
        // j oznacza drugi koniec potencjalnej krawedzi
        for(int j = i + 1; j < vertexCount; ++j)
        {
            // do wektora mozliwych krawedzi dodaje te dwa wierzcholki
            possibleEdges.emplace_back(i, j);
        }
    }
    // w ten sposob generuje mozliwe krawedzie bez powtorzen w wektorze
    // nie bedzie krawedzi ktora jako poczatek bedzie miala "x" i jako koniec "x"


    //wektor z mozliwymi polaczeniami tasuje shuffle przestawia wszystkie elementy wektora
    // w losowej kolejnosci
    std::shuffle(possibleEdges.begin(), possibleEdges.end(), gen);

    for(int i = 0; i < edgeCount/2 && i < possibleEdges.size(); ++i)
    {
        int weight = weightDist(gen);
        const auto& [v1, v2] = possibleEdges[i];
        outFile << v1 << " " << v2 << " " << weight << "\n";
        outFile << v2 << " " << v1 << " " << weight << "\n";
    }
}

void displayMenu()
{
    std::cout << "\n=== GRAPH MENU ===\n";
    std::cout << "1. Load graph from file\n";
    std::cout << "2. Add vertex\n";
    std::cout << "3. Add edge\n";
    std::cout << "4. Remove vertex\n";
    std::cout << "5. Remove edge\n";
    std::cout << "6. Check if vertices are adjacent\n";
    std::cout << "7. Find opposite vertex\n";
    std::cout << "8. Display vertices\n";
    std::cout << "9. Display edges\n";
    std::cout << "10. Display incident edges\n";
    std::cout << "11. Display graph\n";
    std::cout << "12. Generate random graph\n";
    std::cout << "0. Exit\n";
    std::cout << "Enter choice: ";
}

void handleUserInputList(std::unique_ptr<Graph>& graph)
{
    int choice;
    do
    {
        displayMenu();
        std::cin >> choice;

        try
        {
            switch(choice)
            {
                case 1:
                {
                    std::string filename = "../../../"; 
                    std::string txt = ".txt";
                    std::string name;
                    std::cout << "Enter file name: ";
                    std::cin >> name;
                    std::string filePath = filename + name + txt;
                    std::ifstream file(filePath);
                    graph = AdjacencyListGraph::createGraph(file);
                    std::cout << "Graph loaded successfully!\n";
                    break;
                }
                case 2:
                {
                    int value;
                    std::cout << "Enter vertex value: ";
                    std::cin >> value;
                    int id = graph->insertVertex(value);
                    std::cout << "Added vertex with ID: " << id << "\n";
                    break;
                }
                case 3:
                {
                    int v1, v2, weight;
                    std::cout << "Enter vertices and weight (v1 v2 weight): ";
                    std::cin >> v1 >> v2 >> weight;
                    int edgeId = graph->insertEdge(v1, v2, weight);
                    std::cout << "Added edge with ID: " << edgeId << "\n";
                    break;
                }
                case 4:
                {
                    int v;
                    std::cout << "Enter vertex ID to remove: ";
                    std::cin >> v;
                    graph->removeVertex(v);
                    std::cout << "Removed vertex " << v << "\n";
                    break;
                }
                case 5:
                {
                    int e;
                    std::cout << "Enter edge ID to remove: ";
                    std::cin >> e;
                    graph->removeEdge(e);
                    std::cout << "Removed edge " << e << "\n";
                    break;
                }
                case 6:
                {
                    int v1, v2;
                    std::cout << "Enter two vertices (v1 v2): ";
                    std::cin >> v1 >> v2;
                    bool adjacent = graph->areAdjacent(v1, v2);
                    std::cout << "Vertices " << v1 << " and " << v2 << (adjacent ? " ARE " : " ARE NOT ")
                              << "adjacent\n";
                    break;
                }
                case 7:
                {
                    int v, e;
                    std::cout << "Enter vertex and edge (v e): ";
                    std::cin >> v >> e;
                    int opposite = graph->opposite(v, e);
                    std::cout << "Opposite vertex: " << opposite << "\n";
                    break;
                }
                case 8:
                {
                    auto vertices = graph->showVertices();
                    std::cout << "Vertices: ";
                    for(int v : vertices)
                        std::cout << v << " ";
                    std::cout << "\n";
                    break;
                }
                case 9:
                {
                    auto edges = graph->showEdges();
                    std::cout << "Edges: ";
                    for(int e : edges)
                        std::cout << e << " ";
                    std::cout << "\n";
                    break;
                }
                case 10:
                {
                    int v;
                    std::cout << "Enter vertex: ";
                    std::cin >> v;
                    std::cout << "\nIncident edges: ";
                    auto incident = graph->incidentEdges(v);
                    for (int e : incident)
                    {
                        std::cout << e << " "; 
                    }
                    break;
                }
                case 11:
                {
                    auto* alg = dynamic_cast<AdjacencyListGraph*>(graph.get());
                    if(alg)
                        alg->printGraph();
                    else
                        std::cout << "Cannot display graph\n";
                    break;
                }
                case 12:
                {

                    int v, density;
                    std::cout << "Enter number of vertices ";
                    std::cin >> v;
                    std::cout << "\nEnter number of edges ";
                    std::cin >> density;
                    try
                    {
                        generateRandomGraph("../../../RandomGraph.txt", v, density);
                    

                    }
                    catch (const std::exception& e)
                    {
                        std::cerr << "Error: " << e.what()<<"\n";
                    }

                    break;
                }

                case 0:
                    std::cout << "Exiting program\n";
                    break;
                default:
                    std::cout << "Invalid choice\n";
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << "Error: " << e.what() << "\n";
        }
        system("pause");
        system("cls");
    } while(choice != 0);
}

void handleUserInputMatrix(std::unique_ptr<Graph>& graph)
{
    int choice;
    do
    {
        displayMenu();
        std::cin >> choice;

        try
        {
            switch(choice)
            {
                case 1:
                {
                    std::string filename = "../../../";
                    std::string txt = ".txt";
                    std::string name;
                    std::cout << "Enter file name: ";
                    std::cin >> name;
                    std::string filePath = filename + name + txt;
                    std::ifstream file(filePath);
                    graph = AdjacencyMatrixGraph::createGraph(file);
                    std::cout << "Graph loaded successfully!\n";
                    break;
                }
                case 2:
                {
                    int value;
                    std::cout << "Enter vertex value: ";
                    std::cin >> value;
                    int id = graph->insertVertex(value);
                    std::cout << "Added vertex with ID: " << id << "\n";
                    break;
                }
                case 3:
                {
                    int v1, v2, weight;
                    std::cout << "Enter vertices and weight (v1 v2 weight): ";
                    std::cin >> v1 >> v2 >> weight;
                    int edgeId = graph->insertEdge(v1, v2, weight);
                    std::cout << "Added edge with ID: " << edgeId << "\n";
                    break;
                }
                case 4:
                {
                    int v;
                    std::cout << "Enter vertex ID to remove: ";
                    std::cin >> v;
                    graph->removeVertex(v);
                    std::cout << "Removed vertex " << v << "\n";
                    break;
                }
                case 5:
                {
                    int e;
                    std::cout << "Enter edge ID to remove: ";
                    std::cin >> e;
                    graph->removeEdge(e);
                    std::cout << "Removed edge " << e << "\n";
                    break;
                }
                case 6:
                {
                    int v1, v2;
                    std::cout << "Enter two vertices (v1 v2): ";
                    std::cin >> v1 >> v2;
                    bool adjacent = graph->areAdjacent(v1, v2);
                    std::cout << "Vertices " << v1 << " and " << v2 << (adjacent ? " ARE " : " ARE NOT ")
                              << "adjacent\n";
                    break;
                }
                case 7:
                {
                    int v, e;
                    std::cout << "Enter vertex and edge (v e): ";
                    std::cin >> v >> e;
                    int opposite = graph->opposite(v, e);
                    std::cout << "Opposite vertex: " << opposite << "\n";
                    break;
                }
                case 8:
                {
                    auto vertices = graph->showVertices();
                    std::cout << "Vertices: ";
                    for(int v : vertices)
                        std::cout << v << " ";
                    std::cout << "\n";
                    break;
                }
                case 9:
                {
                    auto edges = graph->showEdges();
                    std::cout << "Edges: ";
                    for(int e : edges)
                        std::cout << e << " ";
                    std::cout << "\n";
                    break;
                }
                case 10:
                {
                    int v;
                    std::cout << "Enter vertex: ";
                    std::cin >> v;
                    std::cout << "\nIncident edges: ";
                    auto incident = graph->incidentEdges(v);
                    for(int e : incident)
                    {
                        std::cout << e << " ";
                    }
                    break;
                }
                case 11:
                {
                    auto* alg = dynamic_cast<AdjacencyMatrixGraph*>(graph.get());
                    if(alg)
                        alg->printGraph();
                    else
                        std::cout << "Cannot display graph\n";
                    break;
                }
                case 12:
                {

                    int v, density;
                    std::cout << "Enter number of vertices ";
                    std::cin >> v;
                    std::cout << "\nEnter number of edges ";
                    std::cin >> density;
                    try
                    {
                        generateRandomGraph("../../../RandomGraph.txt", v, density);
                    }
                    catch(const std::exception& e)
                    {
                        std::cerr << "Error: " << e.what() << "\n";
                    }

                    break;
                }

                case 0:
                    std::cout << "Exiting program\n";
                    break;
                default:
                    std::cout << "Invalid choice\n";
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << "Error: " << e.what() << "\n";
        }
        system("pause");
        system("cls");
    } while(choice != 0);
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);


    std::unique_ptr<Graph> graphMatrix = std::make_unique<AdjacencyMatrixGraph>();
    handleUserInputMatrix(graphMatrix);



    std::unique_ptr<Graph> graphList = std::make_unique<AdjacencyListGraph>();
    handleUserInputList(graphList);
    return 0;
}