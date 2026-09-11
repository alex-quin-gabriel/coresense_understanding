#ifndef vampire_interface_hpp
#define vampire_interface_hpp

#include <unordered_map>
#include <vector>
#include <iostream>
#include <string>
#include <regex>

#include "tinyxml2.h"

#include "coresense_understanding_system/model.hpp"
#include "coresense_understanding_system/understanding_graph.hpp"


namespace ns_graph = coresense::understanding::graph;


namespace coresense::understanding::interfaces::vampire {

class VampireInterface {

private:
  std::string build_behavior_tree(std::unordered_map<std::string, std::shared_ptr<ns_graph::GraphNode>> map, std::string root_id);
  std::string consume_answer(std::map<std::string, coresense::understanding::model::Engine> engines, std::unordered_map<std::string, std::shared_ptr<ns_graph::GraphNode>> & map, std::string & answer);
  //bool consume_exertn(std::map<std::string, coresense::understanding::model::Engine> engines, std::unordered_map<std::string, std::shared_ptr<ns_graph::GraphNode>> & map, std::string & answer);
  void add_engine_node(tinyxml2::XMLDocument doc, tinyxml2::XMLElement* root, tinyxml2::XMLElement* target_node, std::string engine_name, std::string ros_pkg, std::string path);
  tinyxml2::XMLElement * add_parallel_node(tinyxml2::XMLDocument doc, tinyxml2::XMLElement* target_node);
  tinyxml2::XMLElement * add_sequence_node(tinyxml2::XMLDocument doc, tinyxml2::XMLElement* target_node);
public:
  VampireInterface() {};
  //std::map<std::string, std::string> parse_output(std::map<std::string, coresense::understanding::model::Engine> engines, std::string output);
  std::map<std::string, std::shared_ptr<ns_graph::GraphNode>> parse_output(std::map<std::string, coresense::understanding::model::Engine> engines, std::string output);
  std::string build_behavior_tree(std::shared_ptr<ns_graph::GraphNode> root);
  
};

} // namespace 'coresense::understanding::interfaces::vampire' ends
#endif
