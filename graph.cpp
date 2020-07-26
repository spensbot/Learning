#include <vector>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <vector>

struct Node
{
  Node(const std::string& name) : name(name) {} 
  std::string name;
};

struct Edge
{
  Edge(Node* from, Node* to, float weight) : from(from), to(to), weight(weight) {}
  Node* from;
  Node* to;
  float weight;
};

class Graph
{
public:

  void populate(){
    addNode("London");
    addNode("Iceland");
    addNode("Barcelona");
    addNode("Portland");
    addNode("New York City");
    addNode("Las Vegas");
    addNode("Seattle");
    addNode("Los Angeles");
    addNode("Miami");
    addEdgeTwoWay("Portland", "Las Vegas", 1.7f);
    addEdgeTwoWay("Portland", "Seattle", 1.3f);
    addEdgeTwoWay("Seattle", "New York City", 4.1f);
    addEdgeTwoWay("New York City", "London", 9.2f);
    addEdgeTwoWay("London", "Barcelona", 2.2f);
    addEdgeTwoWay("Los Angeles", "Las Vegas", 1.2f);
    addEdgeTwoWay("Los Angeles", "Portland", 2.3f);
    addEdgeTwoWay("Los Angeles", "Seattle", 3.2f);
    addEdgeTwoWay("Los Angeles", "New York City", 5.1f);
    addEdgeTwoWay("Los Angeles", "Barcelona", 13.2f);
    addEdgeTwoWay("Miami", "Las Vegas", 3.2f);
    addEdgeTwoWay("Miami", "New York City", 2.3f);
    addEdgeTwoWay("Miami", "Barcelona", 9.4f);
  }

  void addNode(const std::string& name){
    nodes.insert(std::make_pair(name, Node(name)));
  }

  Node* getNode(const std::string& name){
    auto nodeIterator = nodes.find(name);
    if (nodeIterator != nodes.end()){
      return &(nodeIterator->second);
    }
    return nullptr;
  }

  void addEdgeOneWay(const std::string& from, const std::string& to, float weight){
    auto fromNode = getNode(from);
    auto toNode = getNode(to);
    if (fromNode == nullptr){
      std::cout << "Error: Can't find the node " << from << std::endl;
    } else if (toNode == nullptr){
      std::cout << "Error: Can't find the node " << to << std::endl;
    } else {
      edges.insert(std::make_pair(from, Edge(fromNode, toNode, weight)));
    }
  }

  void addEdgeTwoWay(const std::string& from, const std::string& to, float weight){
    addEdgeOneWay(from, to, weight);
    addEdgeOneWay(to, from, weight);
  }

  void printConnections(const std::string& startName){
    auto edgeRange = edges.equal_range(startName);
    std::cout << startName << " connections: " << std::endl;
    for (auto i = edgeRange.first ; i != edgeRange.second ; i++){
      std::cout << i->second.to->name << " -> " << i->second.weight << " hours" << std::endl;
    }
  }

  bool shallowSearch(const std::string& startName, const std::string& endName){
    auto edgeRange = edges.equal_range(startName);
    for (auto i = edgeRange.first ; i != edgeRange.second ; i++){
      if (i->second.to->name == endName){
        return true;
      }
    }
    return false;
  }

  bool depthFirstSearch(const std::string& startName, const std::string& endName){
    // Create a hash set of visited nodes
    std::unordered_set<Node*> visited;
    // Create a stack of nodes to visit
    std::vector<Node*> toVisit;

    // Get pointers to the start and end nodes
    auto startNode = getNode(startName);
    auto endNode = getNode(endName);
    if (startNode == nullptr || endNode == nullptr){
      std::cout << "Error: Invalid start or end name" << std::endl;
      return false;
    }

    // Add the first node to the stack
    toVisit.push_back(startNode);

    while(toVisit.size() > 0){
      // Get the next node from the stack
      Node* current = toVisit.back();
      toVisit.pop_back();
      std::cout << current->name << std::endl;
      // Iterate through all unvisited nodes connected to "current"
      auto edgeRange = edges.equal_range(current->name);
      for (auto i = edgeRange.first ; i != edgeRange.second ; i++){
        Node* next = i->second.to;
        std::cout << "  " << next->name << std::endl;
        if (next->name == endName){
          // We found a path from start to end
          return true;
        } else {
          if (visited.count(next) == 0){
            toVisit.push_back(next);
            visited.insert(next);
          }
        }
      }
    }
    return false;
  }

  void printPath(const std::vector<Edge*>& path){
    float distance = 0.0f;
    for (Edge* edge : path){
      // std::cout << edge->from->name << " - " << edge->weight << " -> ";
      std::cout << edge->from->name << " -> ";
      distance += edge->weight;
    }
    std::cout << path.back()->to->name << " | " << distance << std::endl;
  }

  bool depthFirstSearch2(const std::string& startName, const std::string& endName){
    // Create a hash set of visited edges
    std::unordered_set<Edge*> traveled;
    // Create a stack of nodes to visit. Each item also contains a vector of edges which led to that node
    std::vector<std::pair<Node*, std::vector<Edge*>>> toVisit;
    
    std::cout << "Searching for Connections between " << startName << " and " << endName << "..." << std::endl;

    // Get pointers to the start and end nodes
    auto startNode = getNode(startName);
    auto endNode = getNode(endName);
    if (startNode == nullptr || endNode == nullptr){
      std::cout << "Error: Invalid start or end name" << std::endl;
      return false;
    }

    // Add the first node to the stack
    toVisit.push_back(std::make_pair(startNode, std::vector<Edge*>()));

    bool isConnected = false;

    while(toVisit.size() > 0){
      // Get the next node from the stack
      auto current = toVisit.back();
      auto currentNode = current.first;
      auto currentPath = current.second;
      toVisit.pop_back();
      // Iterate through all untraveled nodes connected to "current"
      auto edgeRange = edges.equal_range(currentNode->name);
      for (auto i = edgeRange.first ; i != edgeRange.second ; i++){
        Edge* edge = &(i->second);
        Node* nextNode = edge->to;
        
        auto newPath = std::vector<Edge*>(currentPath);
        newPath.push_back(edge);

        if (nextNode->name == endName){
          // We found a path from start to end
          isConnected = true;
          printPath(newPath);
        } else {
          if (traveled.count(edge) == 0){
            toVisit.push_back(std::make_pair(nextNode, newPath));
            traveled.insert(edge);
          }
        }
      }
    }
    if (!isConnected){
      std::cout << "There are none :/"<< std::endl;
    }
    return isConnected;
  }

  bool breadthFirstSearch(const std::string& startName, const std::string& endName){
    // Create a hash set of visited nodes
    std::unordered_set<Node*> visited;
    // Create a fifo queue of nodes to visit
    std::deque<Node*> toVisit;

    // Get pointers to the start and end nodes
    auto startNode = getNode(startName);
    auto endNode = getNode(endName);
    if (startNode == nullptr || endNode == nullptr){
      std::cout << "Error: Invalid start or end name" << std::endl;
      return false;
    }

    // Add the first node to the queue
    toVisit.push_back(startNode);

    while(toVisit.size() > 0){
      // Get the next node from the front of the queue
      Node* current = toVisit.front();
      toVisit.pop_front();
      std::cout << current->name << std::endl;
      // Iterate through all unvisited nodes connected to "current"
      auto edgeRange = edges.equal_range(current->name);
      for (auto i = edgeRange.first ; i != edgeRange.second ; i++){
        Node* next = i->second.to;
        std::cout << "  " << next->name << std::endl;
        if (next->name == endName){
          // We found a path from start to end
          return true;
        } else {
          if (visited.count(next) == 0){
            toVisit.push_back(next);
            visited.insert(next);
          }
        }
      }
    }
    return false;
  }

  bool breadthFirstSearch2(const std::string& startName, const std::string& endName){
    // Create a hash set of visited edges
    std::unordered_set<Edge*> traveled;
    // Create a fifo queue of nodes to visit. Each item also contains a vector of edges which led to that node
    std::deque<std::pair<Node*, std::vector<Edge*>>> toVisit;

    std::cout << "Searching for Connections between " << startName << " and " << endName << "..." << std::endl;

    // Get pointers to the start and end nodes
    auto startNode = getNode(startName);
    auto endNode = getNode(endName);
    if (startNode == nullptr || endNode == nullptr){
      std::cout << "Error: Invalid start or end name" << std::endl;
      return false;
    }

    // Add the first node to the stack
    toVisit.push_back(std::make_pair(startNode, std::vector<Edge*>()));

    bool isConnected = false;

    while(toVisit.size() > 0){
      // Get the next node from the stack
      auto current = toVisit.front();
      auto currentNode = current.first;
      auto currentPath = current.second;
      toVisit.pop_front();
      // Iterate through all untraveled nodes connected to "current"
      auto edgeRange = edges.equal_range(currentNode->name);
      for (auto i = edgeRange.first ; i != edgeRange.second ; i++){
        Edge* edge = &(i->second);
        Node* nextNode = edge->to;
        
        auto newPath = std::vector<Edge*>(currentPath);
        newPath.push_back(edge);

        if (nextNode->name == endName){
          // We found a path from start to end
          isConnected = true;
          printPath(newPath);
        } else {
          if (traveled.count(edge) == 0){
            toVisit.push_back(std::make_pair(nextNode, newPath));
            traveled.insert(edge);
          }
        }
      }
    }
    if (!isConnected){
      std::cout << "There are none :/" << std::endl;
    }
    return isConnected;
  }

private:
  std::unordered_map<std::string, Node> nodes;
  std::unordered_multimap<std::string, Edge> edges;
};


int main(){
  Graph flights;
  flights.populate();

  std::cout << std::endl << "---------------------------------------------------------------------------------------" << std::endl;
  std::cout << "Depth-First-Search. Unlimited Node Visits. Limited Connection Uses." << std::endl;
  std::cout << "---------------------------------------------------------------------------------------" << std::endl << std::endl; 
  flights.depthFirstSearch2("Portland", "New York City");
  std::cout << std::endl; 
  flights.depthFirstSearch2("Los Angeles", "London");
  std::cout << std::endl; 
  flights.depthFirstSearch2("Portland", "Iceland");

  std::cout << std::endl << "---------------------------------------------------------------------------------------" << std::endl;
  std::cout << "Breadth-First-Search. Unlimited Node Visits. Limited Connection Uses." << std::endl;
  std::cout << "---------------------------------------------------------------------------------------" << std::endl << std::endl; 
  flights.breadthFirstSearch2("Portland", "New York City");
  std::cout << std::endl; 
  flights.breadthFirstSearch2("Los Angeles", "London");
  std::cout << std::endl; 
  flights.breadthFirstSearch2("Portland", "Iceland");
  std::cout << std::endl; 

  return 1;
}