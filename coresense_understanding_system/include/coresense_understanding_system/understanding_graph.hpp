#ifndef understanding_graph_hpp
#define understanding_graph_hpp

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <regex>

#include "uuid/uuid.h"
#include "coresense_understanding_system/model.hpp"

namespace coresense::understanding::graph {

  // Classes
class GraphNode {
protected:
  virtual bool isSimilarTo(const GraphNode& other) const=0;
  friend bool similar(const std::shared_ptr<GraphNode>& lhs, const std::shared_ptr<GraphNode>& rhs);
public:
  std::string id;
  std::string name;
  
  GraphNode();
  ~GraphNode() {};
  virtual std::string print()=0;
  virtual std::string get_id()=0;
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
protected:
  bool isSimilarTo(const GraphNode& other) const override;
  friend bool similar(const std::shared_ptr<GraphNode>& lhs, const std::shared_ptr<GraphNode>& rhs);
public:
  std::string formalism;
  ConceptNode(std::string modelet_name, std::string formalism);
  ConceptNode(coresense::understanding::model::Modelet modelet);
  ~ConceptNode() {};
  std::string print() override;
  std::string get_id() override;
  std::string get_formalism();
  std::string print_kb_fetch();
};

class ExertnNode : public GraphNode {
protected:
  bool isSimilarTo(const GraphNode& other) const override;
  friend bool similar(const std::shared_ptr<GraphNode>& lhs, const std::shared_ptr<GraphNode>& rhs);
public:
  coresense::understanding::model::Engine engine;
  std::vector<std::string> modelets;
  std::vector<std::shared_ptr<GraphNode>> children;
  std::vector<std::string> exerts;
  
  ExertnNode(coresense::understanding::model::Engine engine);
  ~ExertnNode() {};
  std::string print() override;
  std::string get_id() override;
  std::string get_bt_line();
  void add_node(std::shared_ptr<GraphNode> & node);
};

bool similar(const std::shared_ptr<GraphNode>& lhs, const std::shared_ptr<GraphNode>& rhs);

} // end of namespace coresense::understanding::graph
#endif
