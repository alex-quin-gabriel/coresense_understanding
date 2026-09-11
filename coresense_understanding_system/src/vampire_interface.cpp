#include <iostream>
#include "coresense_understanding_system/vampire_interface.hpp"

namespace coresense::understanding::interfaces::vampire {

std::map<std::string, std::shared_ptr<ns_graph::GraphNode>> VampireInterface::parse_output(std::map<std::string, coresense::understanding::model::Engine> engines, std::string output) {
  //std::map<std::string, std::string> trees;
  std::map<std::string, std::shared_ptr<ns_graph::GraphNode>> trees;
  const std::regex answer_line_regex = std::regex("^% SZS answers Tuple \\[(.*)\\|_\\] for");
  const std::regex answer_regex = std::regex("\\[\\w->(.*?)\\]\\|?");
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLElement* rootXML = doc.NewElement("root");
  rootXML->SetAttribute("BTCPP_format", 4);
  doc.InsertFirstChild(rootXML);
  std::string line;
  std::stringstream string_handle;

  string_handle << output;
  while (std::getline(string_handle, line, '\n') ) {
    //extract includes
    std::smatch answer_match;
    std::unordered_map<std::string, std::shared_ptr<ns_graph::GraphNode>> collected_nodes;
    if (std::regex_search(line, answer_match, answer_line_regex)) {
      std::string answers = answer_match[1];
      const std::sregex_token_iterator End;
      for (std::sregex_token_iterator it(answers.begin(), answers.end(), answer_regex, 1); it != End; ++it) {
        std::string answer = *it;
        consume_answer(engines, collected_nodes, answer);
        std::cout << "Finished Consuming Answer, building tree" << std::endl;
        trees["Understanding_Solution_" + answer] = collected_nodes[answer]; //build_behavior_tree(map, answer);
      }
    } else {
      // silently handle no match
    }
  }
  return trees;
}

std::string VampireInterface::build_behavior_tree(std::unordered_map<std::string, std::shared_ptr<ns_graph::GraphNode>> map, std::string root_id) {
  return "<root BTCPP_format=\"4\">\n<BehaviorTree ID=\"Understanding_Solution_" + root_id + "\">\n<Sequence>\n" + map[root_id]->print() + "\n</Sequence>\n</BehaviorTree>\n</root>";
}

std::string VampireInterface::build_behavior_tree(std::shared_ptr<ns_graph::GraphNode> root) {
  return "<root BTCPP_format=\"4\">\n<BehaviorTree ID=\"Understanding_Solution_" + root->id + "\">\n<Sequence>\n" + root->print() + "\n</Sequence>\n</BehaviorTree>\n</root>";
}


std::string VampireInterface::consume_answer(std::map<std::string, coresense::understanding::model::Engine> engines, std::unordered_map<std::string, std::shared_ptr<ns_graph::GraphNode>> & collected_nodes, std::string & answer) {
  // This reads exert[0-9]+(engine, modelet1, modelet2, ..., modeletN) string and turns it into an object representation, recursively from right to left (roughly) as the parameter side of the regex does not include brackets.
  const std::regex exert_regex = std::regex("exert(\\d+)\\('(coresense:engine:[\\w-]+)',([':,\\w-]+)\\)");
  const std::regex inputs_regex = std::regex("'?([:\\w-]+)'?");
  std::smatch match;
  if (std::regex_search(answer, match, exert_regex)) {
    // we found an engine exertion

    //TODO SECONDARY PRIORITY: how do we decide whether an engine was run yet
    // idea: create an id based on used inputs
    // not sure this scales to complex cases though (ones where the same engine is run with different configurations)
    // we'll have to test this
      // create a new exert node
    auto exert_node = std::make_shared<ns_graph::ExertnNode>(engines[match[2]]); 
    std::cout << "created ExertnNode " << exert_node->id << " with name " << exert_node->name << " from " << match[0] << std::endl;
    answer = std::regex_replace(answer, exert_regex, exert_node->id, std::regex_constants::format_first_only);
    // record new node, it might be used more often in this tree, index by the id we just replaced it with.
    collected_nodes[exert_node->id] = exert_node;
    // get the parameters from the match
    std::string inputs = match[3];
    // get iterator over engine inputs from which we get the input formalisms
    auto input_template_iterator = exert_node->engine.inputs.begin();
    const std::sregex_token_iterator End;
    for (std::sregex_token_iterator it(inputs.begin(), inputs.end(), inputs_regex, 1); it != End; ++it) {
      //get formalism
      std::string formalism = (*input_template_iterator).formalism; ++input_template_iterator;
      //get parameter. this is either an original string describing an exertion or modelet, or it is one of the exertion ids we added by regex_replace a few lines earlier
      std::string input = *it;
      if (collected_nodes.find(input) == collected_nodes.end()) { // if the input is not yet recorded we need to process it
        // this has to be a modelet because the regex and adding the node above ensure that exerts are added before we get here.
        auto position = input.rfind(":")+1;
        auto size = input.size() - position;
        auto modelet = std::make_shared<ns_graph::ConceptNode>(input.substr(position, size), formalism);
        collected_nodes[modelet->id] = modelet; 
        std::cout << "Created ConceptNode " << modelet->id << " with name " << collected_nodes[modelet->id]->name << " with formalism "<< formalism << std::endl;
        exert_node->add_node(collected_nodes[modelet->id]);
      } else { // if the input was recorded, we just add that previous node
        exert_node->add_node(collected_nodes[input]);
      }
    }
    return consume_answer(engines, collected_nodes, answer);
  } else {
    return answer;
  }
}

tinyxml2::XMLElement * VampireInterface::add_parallel_node(tinyxml2::XMLDocument doc, tinyxml2::XMLElement* target_node) {
  tinyxml2::XMLElement * parallel_node = doc.NewElement("Parallel");
  target_node->InsertEndChild(parallel_node);
  return parallel_node;
}

tinyxml2::XMLElement * VampireInterface::add_sequence_node(tinyxml2::XMLDocument doc, tinyxml2::XMLElement* target_node) {
  tinyxml2::XMLElement * sequence_node = doc.NewElement("Sequence");
  target_node->InsertEndChild(sequence_node);
  return sequence_node;
}

void VampireInterface::add_engine_node(tinyxml2::XMLDocument doc, tinyxml2::XMLElement* root, tinyxml2::XMLElement* target_node, std::string engine_name, std::string ros_pkg, std::string path) {
  tinyxml2::XMLElement * subtree_node = doc.NewElement("Subtree");
  subtree_node->SetAttribute("ID", engine_name.c_str());
  target_node->InsertEndChild(subtree_node);
  tinyxml2::XMLElement * include_node = doc.NewElement("include");
  include_node->SetAttribute("ros_pkg", ros_pkg.c_str());
  include_node->SetAttribute("path", path.c_str());
  root->InsertFirstChild(include_node);
}

} // namespace 'coresense::understanding::interfaces::vampire' ends
