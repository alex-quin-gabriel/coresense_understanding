
#include "uuid/uuid.h"
#include <iostream>
#include <algorithm>
#include "coresense_understanding_system/understanding_graph.hpp"

using namespace coresense::understanding::graph;

GraphNode::GraphNode() {
  uuid_t uuid;
  char tmp[37];
  uuid_generate(uuid);
  uuid_unparse(uuid, tmp);
  id = std::string(tmp);
}

ConceptNode::ConceptNode(coresense::understanding::model::Modelet modelet)
  : GraphNode() {
  name = modelet.name;
  formalism = modelet.formalism;
}

ConceptNode::ConceptNode(std::string modelet_name, std::string formalism)
  : GraphNode(), formalism(formalism) {
  name = modelet_name;
}

std::string ConceptNode::print() {
  std::ostringstream tree;
  std::string _formalism = formalism.substr(formalism.rfind(':')+1);
  std::replace(_formalism.begin(), _formalism.end(), '/', '_');
  tree << "<SubTree ID=\"Get" << _formalism << "Modelet\" modelet_id=\"<coresense:modelet:" << name << ">\" modelet=\"{" << name << "_output_" << id << "}\"/>";
  return tree.str();
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
  children.push_back(node);
  if (node->get_id().rfind("MODELET_", 0) == 0) {
    modelets.push_back(node->id);
  } else if (node->get_id().rfind("EXERT_", 0) == 0) {
    exerts.push_back(node->id);
    modelets.push_back(node->id);
  }
}

std::string ExertnNode::print() {
  std::ostringstream tree;
  bool multiple_children = children.size() > 1;
  if (multiple_children) {
    tree << "<Parallel failure_count=\"1\" success_count=\"" << children.size() << "\">" << std::endl;
  }
  for (const std::shared_ptr<GraphNode> child : children) {
    if (multiple_children) {
      tree << "<Sequence>";
    }
    tree << child->print();
    if (multiple_children) {
      tree << "</Sequence>" << std::endl;
    } else {
      tree << std::endl;

    }
//    if (typeid(*child) == typeid(ExertnNode)) {
//     auto child_engine = std::dynamic_pointer_cast<ExertnNode>(child);
//      tree << child_engine->print();
      // exert child
//    } else if (typeid(*child) == typeid(ConceptNode)) {
//      auto child_concept = std::dynamic_pointer_cast<ConceptNode>(child);
//      tree << child_concept.get_bt_line();
//    }
  }
  if (multiple_children) {
    tree << "</Parallel>" << std::endl;
  }
  tree << get_bt_line();
  return tree.str();
}

std::string ExertnNode::get_bt_line() {
  std::ostringstream tree;
  tree << "<SubTree ID=\"" << name << "\" " << engine.engine_output.name.substr(engine.engine_output.name.rfind(':')+1) << "=\"{" << name << "_output_" << id << "}\" ";
  int count = 0;
  for (auto child : children) {
    std::string name = engine.inputs[count++].name;
    name = name.substr(name.rfind(":")+1);
    tree << name << "=\"{" << child->name << "_output_" << child->id << "}\" ";
  }
  tree << "/>";
  return tree.str();
}

std::string ExertnNode::get_id() {
  return "EXERT_" + name + "_" + id;
}

namespace coresense::understanding::graph {
bool similar(const std::shared_ptr<GraphNode>& lhs, const std::shared_ptr<GraphNode>& rhs) {
  return typeid(*lhs) == typeid(*rhs) && lhs->isSimilarTo(*rhs);
}

}

bool ConceptNode::isSimilarTo(const GraphNode& other) const {
  auto other_engine = dynamic_cast<const ConceptNode&>(other);
  return formalism == other_engine.formalism;
}


bool ExertnNode::isSimilarTo(const GraphNode& other) const {
  auto other_engine = dynamic_cast<const ExertnNode&>(other);

  return name == other_engine.name && std::equal(children.begin(), children.end(), other_engine.children.begin(), similar);
}
