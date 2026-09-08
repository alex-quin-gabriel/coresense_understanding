
#include "uuid/uuid.h"
#include <iostream>
#include <algorithm>
#include "coresense_understanding/understanding_graph.hpp"

using namespace coresense::understanding::graph;

GraphNode::GraphNode() {
  uuid_t uuid;
  char tmp[37];
  uuid_generate(uuid);
  uuid_unparse(uuid, tmp);
  id = std::string(tmp);
}


ConceptNode::ConceptNode(std::string modelet_name, std::string formalism)
  : GraphNode(), formalism(formalism) {
  name = modelet_name;
}

std::string ConceptNode::print(std::unordered_map<std::string, std::shared_ptr<GraphNode>> & map) {
  return name;
}

std::string ConceptNode::get_id() {
  return "MODELET_" + name + "_" + id;
}

std::string ConceptNode::get_formalism() {
  return formalism;
}


ExertnNode::ExertnNode(coresense::understanding::model::Engine engine)
  : GraphNode(), engine(engine) {
  name = engine.name.substr(engine.name.rfind(":")+1);
}

void ExertnNode::add_node(std::shared_ptr<GraphNode> & node) {
  if (node->get_id().rfind("MODELET_", 0) == 0) {
    modelets.push_back(node->id);
  } else if (node->get_id().rfind("EXERT_", 0) == 0) {
    exerts.push_back(node->id);
    modelets.push_back(node->id);
  }
}

std::string ExertnNode::print(std::unordered_map<std::string, std::shared_ptr<GraphNode>> & map) {
  std::ostringstream tree;
  //TODO think about where sequence nodes are put
  //  top level sequence
  //  moving in levels down is in fact putting things into the sequence at an earlier spot
  //  so there should only be one sequence? no. if a subtree (in a parallel node) has another subtree, that should be new sequence
  // first: add required subtree exertions, either in parallel or just on a newline in the parent sequence
  for (auto modelet : modelets) {
    auto node = map[modelet];
    if (node->get_id().rfind("MODELET_", 0) == 0) {
      std::string formalism = node->get_formalism();
      // spawn get_formalism_modelet actions for the modelets we have to get from the KB
      formalism = formalism.substr(formalism.rfind(':')+1);
      std::replace(formalism.begin(), formalism.end(), '/', '_');
      tree << "<SubTree ID=\"Get" << formalism << "Modelet\" modelet_id=\"<coresense:modelet:" << node->name << ">\" modelet=\"{" << node->name << "_output_" << node->id << "}\"/>" << std::endl;
    }
  }
  if (exerts.size() > 1) {
    // this should only happen if modelets are in fact exerts
    tree << "<Parallel failure_count=\"1\" success_count=\"" << exerts.size() << "\">" << std::endl;
    for (auto exert : exerts) {
      tree << "<Sequence>" << std::endl;
      tree << map[exert]->print(map) << std::endl;
      tree << "</Sequence>" << std::endl;
    }
    tree << "</Parallel>" << std::endl;
  } else if (!exerts.empty()) {
    tree << map[exerts[0]]->print(map) << std::endl;
  } 
  tree << "<SubTree ID=\"" << name << "\" " << engine.engine_output.name.substr(engine.engine_output.name.rfind(':')+1) << "=\"{" << name << "_output_" << id << "}\" ";
  int count = 0;
  for (auto modelet : modelets) {
    std::string name = engine.inputs[count++].name;
    name = name.substr(name.rfind(":")+1);
    tree << name << "=\"{" << map[modelet]->name << "_output_" << map[modelet]->id << "}\" ";
  }
  tree << "/>";
  return tree.str();
}

std::string ExertnNode::get_id() {
  return "EXERT_" + name + "_" + id;
}

std::string ExertnNode::get_formalism() {
  return "Engines dont have formalisms: " + engine.name;
}
