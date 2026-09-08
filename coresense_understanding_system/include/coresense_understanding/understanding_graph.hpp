#ifndef understanding_graph_hpp
#define understanding_graph_hpp

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <regex>

#include "uuid/uuid.h"
#include <coresense_understanding/model.hpp>

namespace coresense::understanding::graph {

  // Classes
class GraphNode {
public:
  std::string id;
  std::string name;
  
  GraphNode();
  ~GraphNode() {};
  virtual std::string print(std::unordered_map<std::string, std::shared_ptr<GraphNode>> & map)=0;
  virtual std::string get_id()=0;
  virtual std::string get_formalism()=0;
};

//class SubsetNode : public GraphNode {
//public:
//  std::string element;
//  std::string set;
//  
//  SubsetNode(std::smatch match);
//  ~SubsetNode() {};
//  std::string print(std::unordered_map<std::string, std::shared_ptr<GraphNode>> & map) override;
//  std::string get_id() override;
//};

//class ExertNode : public GraphNode {
//public:
//  std::string modelet_set;
//  
//  ExertNode(std::smatch match);
//  ~ExertNode() {};
//  std::string print(std::unordered_map<std::string, std::shared_ptr<GraphNode>> & map) override;
//  std::string get_id() override;
//};

class ConceptNode : public GraphNode {
public:
  std::string formalism;
  ConceptNode(std::string modelet_name, std::string formalism);
  ~ConceptNode() {};
  std::string print(std::unordered_map<std::string, std::shared_ptr<GraphNode>> & map) override;
  std::string get_id() override;
  std::string get_formalism() override;
  std::string print_kb_fetch();
};

class ExertnNode : public GraphNode {
public:
  coresense::understanding::model::Engine engine;
  std::vector<std::string> modelets;
  std::vector<std::string> exerts;
  
  ExertnNode(coresense::understanding::model::Engine engine);
  ~ExertnNode() {};
  std::string print(std::unordered_map<std::string, std::shared_ptr<GraphNode>> & map) override;
  std::string get_id() override;
  std::string get_formalism() override;
  void add_node(std::shared_ptr<GraphNode> & node);
};

}
#endif
