#include "coresense_understanding/model.hpp"
#include <iostream>
namespace coresense::understanding::model {

const std::string CONCEPT = "concept";
const std::string FORMALISM = "formalism";
const std::string REPRESENTATION_CLASS = "representation_class";

std::string create_not_relation1(std::string relation, std::string klass, std::string instance) {
  std::stringstream ss;
  auto position = instance.find_last_of("/:") + 1;
  auto count = instance.find_first_of("#.") - position;
  ss << "tff(not_" << instance.substr(position, count) << "_" << relation << ", axiom,\n  ~" << relation << "('" << instance << "')\n).\n";
  return ss.str();
}

std::string create_relation1(std::string relation, std::string klass, std::string instance) {
  std::stringstream ss;
  auto position = instance.find_last_of("/:") + 1;
  auto count = instance.find_first_of("#.") - position;
  ss << "tff(" << instance.substr(position, count)  << "_" << relation << ", axiom,\n  " << relation << "('"  << instance << "')\n).\n";
  return ss.str();
}

std::string create_relation2(std::string relation, std::string klass1, std::string instance1, std::string klass2, std::string instance2) {
  std::stringstream ss;
  auto position1 = instance1.find_last_of("/:") + 1;
  auto count1 = instance1.find_first_of("#.") - position1;
  auto position2 = instance2.find_last_of("/:") + 1;
  auto count2 = instance2.find_first_of("#.") - position2;
  ss << "tff(" << instance1.substr(position1, count1)  << "_" << instance2.substr(position2, count2)  << "_" << relation << ", axiom,\n  " << relation << "('" << instance1 << "', '" << instance2 << "')\n).\n";
  return ss.str();
}

std::string create_equals_relation2(std::string relation, std::string klass1, std::string instance1, std::string klass2, std::string instance2) {
  std::stringstream ss;
  if (klass2 == "formalism") {
  auto position1 = instance1.find_last_of("/:") + 1;
  auto count1 = instance1.find_first_of("#.") - position1;
  auto position2 = instance2.find_last_of("/:") + 1;
  auto count2 = instance2.find_first_of("#.") - position2;
    ss << "tff(" << instance1.substr(position1, count1)  << "_" << relation << "_" << instance2.substr(position2, count2) << ", axiom,\n  " << relation << "('" << instance1 << "') = '" << instance2 << "'\n).\n";
  } else {
    ss << "tff(" << instance1 << "_" << relation << "_" << instance2 << ", axiom,\n  " << relation << "('" << instance1 << "') = '" << instance2 << "'\n).\n";
  }
  return ss.str();
}

std::string create_equals_relation3(std::string relation, std::string klass1, std::string instance1, std::string klass2, std::string instance2, std::string klass3, std::string instance3) {
  std::stringstream ss;
  auto position1 = instance1.find_last_of("/:") + 1;
  auto count1 = instance1.find_first_of("#.") - position1;
  auto position2 = instance2.find_last_of("/:") + 1;
  auto count2 = instance2.find_first_of("#.") - position2;
  auto position3 = instance3.find_last_of("/:") + 1;
  auto count3 = instance3.find_first_of("#.") - position3;
  ss << "tff(" << instance1.substr(position1, count1)  << "_" << instance2.substr(position2, count2)  << "_" << instance3.substr(position3, count3)  << "_" << relation << ", axiom,\n  " << relation << "('"  << instance1 << "', '" << instance2 << "') = '" << instance3 << "'\n).\n";
  return ss.str();
}

std::string create_relation_limit1(std::string relation, std::string instance_klass, std::string instance, std::string set_klass,  std::set<std::string> set) {
  std::stringstream ss;
  auto position = instance.find_last_of(":/") + 1;
  auto count = instance.find_first_of("#.") - position;
  ss << "tff(axiom_" << instance_klass << "_" << instance.substr(position, count) << "_" << relation << "_limitation, axiom," << std::endl 
     << "  ![X : " << set_klass << "]: " << std::endl 
     << "  (" << std::endl
     << "    " << relation << "('" << instance << "', X)" << std::endl
     << "    =>" << std::endl
     << "    (";
  for (std::string name : set) {
    ss << std::endl <<"      (X = '" << name << "')" << std::endl << "      |"; 
  }
  ss.seekp(-3, ss.cur);
  ss << ")" << std::endl << "  )" << std::endl << ")." << std::endl;
  return ss.str();
}

std::string create_triple_relation_limit_fixed_first(std::string relation, std::string instance_klass, std::string instance, std::set<std::pair<std::string, std::string>> set, std::string set_klass1, std::string set_klass2) {
  std::stringstream ss;
  auto position = instance.find_last_of(":/") + 1;
  auto count = instance.find_first_of("#.") - position;
  ss << "tff(axiom_" << instance_klass << "_" << instance.substr(position, count)  << "_" << relation << "_limitation_fixed_first, axiom," << std::endl 
     << "  ![X : " << set_klass1 << ", Y : " << set_klass2 << "]: " << std::endl 
     << "  (" << std::endl
     << "    " << relation << "('" << instance << "', X, Y)" << std::endl
     << "    =>" << std::endl
     << "    (";
  for (auto entry : set) {
    ss << std::endl <<"      ((X = '" << entry.first << "') & (Y = '" << entry.second << "'))" << std::endl << "      |"; 
  }
  ss.seekp(-3, ss.cur);
  ss << ")" << std::endl << "  )" << std::endl << ")." << std::endl;
  return ss.str();
}
std::string create_triple_relation_limit_fixed_second(std::string relation, std::string instance_klass, std::string instance, std::set<std::pair<std::string, std::string>> set, std::string set_klass1, std::string set_klass2) {
  std::stringstream ss;
  auto position = instance.find_last_of(":/") + 1;
  auto count = instance.find_first_of("#.") - position;
  ss << "tff(axiom_" << instance_klass << "_" << instance.substr(position, count)  << "_" << relation << "_limitation_fixed_second, axiom," << std::endl 
     << "  ![X : " << set_klass1 << ", Y : " << set_klass2 << "]: " << std::endl 
     << "  (" << std::endl
     << "    " << relation << "(X, '" << instance << "', Y)" << std::endl
     << "    =>" << std::endl
     << "    (";
  for (auto entry : set) {
    ss << std::endl <<"      ((X = '" << entry.first << "') & (Y = '" << entry.second << "'))" << std::endl << "      |"; 
  }
  ss.seekp(-3, ss.cur);
  ss << ")" << std::endl << "  )" << std::endl << ")." << std::endl;
  return ss.str();
}

std::string create_relation_exist1(std::string relation, std::string instance_klass, std::string instance, std::string set_klass,  std::set<std::string> set) {
  std::stringstream ss;
  auto position = instance.find_last_of(":/") + 1;
  auto count = instance.find_first_of("#.") - position;
  ss << "tff(axiom_" << instance_klass << "_" << instance.substr(position, count) << "_" << relation << "_existence_left, axiom," << std::endl 
     << "  ![X : " << set_klass << "]: " << std::endl 
     << "  (" << std::endl
     << "    (";
  for (std::string name : set) {
    ss << std::endl <<"      (X = '" << name << "')" << std::endl << "      |"; 
  }
  ss.seekp(-3, ss.cur);
  ss <<     ")" << std::endl
     << "    =>" << std::endl
     << "    " << relation << "('" << instance << "', X)" << std::endl
     << "  )" << std::endl << ")." << std::endl;
  return ss.str();
}

std::string create_relation_exist2(std::string relation, std::string instance_klass, std::string instance, std::string set_klass,  std::set<std::string> set) {
  std::stringstream ss;
  auto position = instance.find_last_of(":/") + 1;
  auto count = instance.find_first_of("#.") - position;
  ss << "tff(axiom_" << instance_klass << "_" << instance.substr(position, count) << "_" << relation << "_existence_right, axiom," << std::endl 
     << "  ![X : " << set_klass << "]: " << std::endl 
     << "  (" << std::endl
     << "    (";
  for (std::string name : set) {
    ss << std::endl <<"      (X = '" << name << "')" << std::endl << "      |"; 
  }
  ss.seekp(-3, ss.cur);
  ss <<     ")" << std::endl
     << "    =>" << std::endl
     << "    " << relation << "(X, '" << instance << "')" << std::endl
     << "  )" << std::endl << ")." << std::endl;
  return ss.str();
}

std::string create_triple_relation_exists(std::string relation, std::string instance_klass, std::string instance, std::set<std::pair<std::string, std::string>> set, std::string set_klass1, std::string set_klass2) {
  std::stringstream ss;
  auto position = instance.find_last_of(":/") + 1;
  auto count = instance.find_first_of("#.") - position;
  ss << "tff(axiom_" << instance_klass << "_" << instance.substr(position, count) << "_" << relation << "_existence_right, axiom," << std::endl 
     << "  ![X : " << set_klass1 << ", Y : " << set_klass2 << "] : " << std::endl 
     << "  (" << std::endl
     << "    (";
  for (auto entry : set) {
    ss << std::endl <<"      ((X = '" << entry.first << "') & (Y = '" << entry.second << "'))" << std::endl << "      |"; 
  }
  ss.seekp(-3, ss.cur);
  ss <<     ")" << std::endl
     << "    =>" << std::endl
     << "    " << relation << "('"  << instance << "', X, Y)" << std::endl
     << "  )" << std::endl << ")." << std::endl;
  return ss.str();
}

std::string create_has_no_relation1(std::string relation, std::string instance_klass, std::string instance, std::string set_klass) {
  std::stringstream ss;
  auto position = instance.find_last_of(":/") + 1;
  auto count = instance.find_first_of("#.") - position;
  ss << "tff(" << instance.substr(position, count) << "_has_no_" << relation <<", axiom, " << std::endl
     << "  ~?[X : "<< set_klass << "]:" << std::endl
     << "    " << relation << "('" << instance << "', X)" << std::endl
     << ")." << std::endl;
  return ss.str();
}

std::string create_has_no_relation2(std::string relation, std::string instance_klass, std::string instance, std::string set_klass) {
  std::stringstream ss;
  auto position = instance.find_last_of(":/") + 1;
  auto count = instance.find_first_of("#.") - position;
  ss << "tff(" << instance.substr(position, count) << "_has_no_" << relation <<", axiom, " << std::endl
     << "  ~?[X : "<< set_klass << "] :" << std::endl
     << "    " << relation << "(X, '" << instance << "')" << std::endl
     << ")." << std::endl;
  return ss.str();
}

std::string create_has_no_triple_relation(std::string relation, std::string instance_klass, std::string instance, std::string set1_klass, std::string set2_klass) {
  std::stringstream ss;
  auto position = instance.find_last_of(":/") + 1;
  auto count = instance.find_first_of("#.") - position;
  ss << "tff(" << instance.substr(position, count) << "_has_no_" << relation <<", axiom, " << std::endl
     << "  ~?[X : " << set1_klass << ", Y : " << set2_klass << " ]:" << std::endl
     << "    " << relation << "('" << instance << "', X, Y)" << std::endl
     << ")." << std::endl;
  return ss.str();
}

void from_json(const nlohmann::json& j, Requirement& r) {
  j.at("klass").get_to(r.klass);
  j.at("value_range").get_to(r.value_range);
  r.klass = "coresense:property:" + r.klass;
}

void from_json(const nlohmann::json& j, Template& t) {
  std::set<std::string> rcs;
  std::set<std::string> concepts;
  j.at("name").get_to(t.name);
  j.at("creator").get_to(t.creator);
  j.at("formalism").get_to(t.formalism);
  j.at("requirements").get_to(t.requirements);
  j.at("concepts").get_to(concepts);
  j.at("representation_classes").get_to(rcs);
  t.name = "coresense:modelet:" + t.name;
  //t.formalism = "coresense:formalism:" + t.formalism;
//  j.at("extents").get_to(t.extents);
//  j.at("locations").get_to(t.locations);
  for (std::string rc : rcs) {
    t.representation_classes.insert("coresense:representation_class:" + rc);
  }
  for (std::string concept : concepts) {
    t.concepts.insert("coresense:concept:" + concept);
  }
}

void from_json(const nlohmann::json& j, Property& p) {
  j.at("klass").get_to(p.klass);
  j.at("value").get_to(p.value);
  p.klass = "coresense:property:" + p.klass;
}

void from_json(const nlohmann::json& j, Modelet& m) {
  std::set<std::string> rcs;
  std::set<std::string> concepts;
  j.at("name").get_to(m.name);
  j.at("formalism").get_to(m.formalism);
  j.at("properties").get_to(m.properties);
  j.at("representation_classes").get_to(rcs);
  j.at("concepts").get_to(concepts);
//  j.at("extents").get_to(m.extents);
//  j.at("locations").get_to(m.locations);
  m.name = "coresense:modelet:" + m.name;
  //m.formalism = "coresense:formalism:" + m.formalism;
  for (std::string rc : rcs) {
    m.representation_classes.insert("coresense:representation_class:" + rc);
  }
  for (std::string concept : concepts) {
    m.concepts.insert("coresense:concept:" + concept);
  }
}

void from_json(const nlohmann::json& j, Resource& r) {
  j.at("name").get_to(r.name);
  j.at("percentage").get_to(r.percentage);
  r.name = "coresense:resource:" + r.name;
}

void from_json(const nlohmann::json& j, Engine& e) {
  j.at("name").get_to(e.name);
  j.at("inputs").get_to(e.inputs);
  j.at("blocked_properties").get_to(e.blocked_properties);
  j.at("time_delay").get_to(e.time_delay);
  j.at("energy_cost").get_to(e.energy_cost);
  j.at("engine_output").get_to(e.engine_output);
  j.at("resources_consumed").get_to(e.resources_consumed);
  j.at("resources_blocked").get_to(e.resources_blocked);
  e.name = "coresense:engine:" +e.name;
}


std::string create_declaration(std::string klass, std::string instance) {
  std::stringstream output;
  //TODO adapt to action/srv/msg formats
  auto position = instance.find_last_of(":/") + 1;
  auto count = instance.find_first_of("#.") - position;
  output << "tff(decl_" << instance.substr(position, count) << "_" << klass << ", type, '"<< instance << "' : " << klass << ").\n";
  return output.str();
}

std::string Template::to_tff(std::string engine) {
  std::stringstream output, req_set;
  output << create_equals_relation2("template_formalism_requirement", "template", engine + "_" + name, "formalism", formalism );
  //output << "tff(" << name << "_formalism_requirement, axiom,\n  template_formalism_requirement(" << name << ") = " << formalism << "\n).\n";
  if (representation_classes.empty()) {
    output << create_has_no_relation1("template_has_representation_class_requirement", "template", engine + "_" + name, "representation_class");
  } else { // template has representation_classes
    output << create_relation_limit1("template_has_representation_class_requirement", "template", engine + "_" + name, "representation_class", representation_classes);
  }
  if (concepts.empty()) {
    output << create_has_no_relation1("template_has_concept_requirement", "template", engine + "_" + name, "concept");
  } else { // template has concepts
    output << create_relation_limit1("template_has_concept_requirement", "template", engine + "_" + name, "concept", concepts);
  }
  if (creator != "") {
    output << create_relation2("template_has_creator_requirement", "template", engine + "_" + name, "engine", creator );
    //output << "tff(" << name << "_template_has_creator_requirement, axiom,\n  template_has_creator_requirement(" << name << ", " << creator << ")\n).\n";
  } else { // template has no creators
    output << create_has_no_relation1("template_has_creator_requirement", "template", engine + "_" + name, "engine");
    //output << "tff(" << name << "_template_has_no_creator_requirement, axiom,\n  ~?[E : engine]:\n    template_has_creator_requirement(" << name << ", E)\n).\n";
  }
  // property requirements
  if (requirements.empty()) { // template has no requirements
    // TODO PRIORITY: this is done preamturely
    output << create_has_no_triple_relation("template_has_property_requirement", "template", engine + "_" + name, "property", "requirement_specification");
  } else { // template has requirements
    std::set<std::pair<std::string, std::string>> entries;
    for (Requirement req : requirements) {
      entries.insert({req.klass, req.value_range});
    }
    output << create_triple_relation_exists("template_has_property_requirement", "template", engine + "_" + name, entries, "property", "requirement_specification");
  }
  return output.str();
}

std::string Modelet::to_tff() {
  std::stringstream output, req_set;
  output << create_declaration("modelet", name);
  output << create_declaration("formalism", formalism);
  //output << "tff(decl_" << name << "_modelet, type,  "<< name << " : modelet).\n";
  //output << "tff(decl_" << formalism << "_formalism, type,  "<< formalism << " : formalism).\n";
  output << create_equals_relation2("formalism_of_modelet", "modelet", name, "formalism", formalism);
  //output << "tff(" << name << "_formalism, axiom,\n  formalism_of_modelet(" << name << ") = " << formalism << "\n).\n";
  if (representation_classes.empty()) {
    output << create_has_no_relation1("modelet_has_representation_class", "modelet", name, "representation_class");
  } else { // modelet has representation_classes
    output << create_relation_limit1("modelet_has_representation_class", "modelet", name, "representation_class", representation_classes);
    output << create_relation_exist1("modelet_has_representation_class", "modelet", name, "representation_class", representation_classes);
  }
  if (concepts.empty()) {
    output << create_has_no_relation1("modelet_models_concept", "modelet", name, "concept");
  } else { // modelet has concepts
    output << create_relation_limit1("modelet_models_concept", "modelet", name, "concept", concepts);
    output << create_relation_exist1("modelet_models_concept", "modelet", name, "concept", concepts);
  }
  if (creator != "") {
    output << create_relation2("modelet_has_creator", "modelet", name, "engine", creator);
    //output << "tff(" << name << "_modelet_has_creator, axiom,\n  modelet_has_creator(" << name << ", " << creator << ")\n).\n";
  } else { // modelet has no creators
    output << create_has_no_relation1("modelet_has_creator", "modelet", name, "engine");
    //output << "tff(" << name << "_modelet_has_no_creator, axiom,\n  ~?[E : engine]:\n    modelet_has_creator(" << name << ", E)\n).\n";
  }
  // properties
  if (properties.empty()) { // modelet has no properties
    output << create_has_no_triple_relation("modelet_has_property", "modelet", name, "property", "value");
  } else { // modelet has properties
    std::set<std::pair<std::string, std::string>> entries;
    for (Property prop : properties) {
      entries.insert({prop.klass, prop.value});
    }
    output << create_triple_relation_exists("modelet_has_property", "modelet", name, entries, "property", "value");
    output << create_triple_relation_limit_fixed_first("modelet_has_property", "modelet", name, entries,  "property", "value");
  }
  return output.str();
}

std::string Engine::to_tff() {
  std::stringstream output, templates;
  //output << "tff(" << name << "_is_available, axiom,\n  ~engine_unavailable(" << name << ")).\n";
  output << create_not_relation1("engine_unavailable", "engine", name);
  //output << "tff(" << name << "_imparts_formalism, axiom,\n  output_modelet_formalism(" << name << ") = " << engine_output.formalism << "\n).\n";
  output << create_equals_relation2("output_modelet_formalism", "engine", name, "formalism", engine_output.formalism);
  if (!inputs.empty()) {
    auto position = name.find_last_of(":/") + 1;
    auto count = name.find_first_of("#.") - position;
    templates << "tff(" << name.substr(position, count) << "_input_definition_axiom, axiom,\n  defines_input" << inputs.size() << "(";
    for (Template templ : inputs) {
      //TODO PRIORITY this can lead to duplicate template definitions when engines define identically named templates
      output << templ.to_tff(name);
      templates << "'" <<  name << "_" << templ.name << "', ";
    }
    templates << "'" << name << "')\n).\n";
    output << templates.str();
  } else { // engine has no inputs
  }
  // These encode the engine->property direction 1->n 
  if (!engine_output.representation_classes.empty()) {
    output << create_relation_exist1("engine_imparts_representation_class", "engine", name, "representation_class", engine_output.representation_classes);
  } else { // engine imparts no representation_classes
    output << create_has_no_relation1("engine_imparts_representation_class", "engine", name, "representation_class");
  }
  if (!engine_output.concepts.empty()) {
    output << create_relation_exist1("engine_imparts_concept", "engine", name, "concept", engine_output.concepts);
  } else { // engine imparts no concepts
    output << create_has_no_relation1("engine_imparts_concept", "engine", name, "concept");
  }
  if (!blocked_properties.empty()) {
    output << create_relation_exist1("engine_blocks_property_transit", "engine", name, "property", blocked_properties);
  } else { // engine imparts no concepts
    output << create_has_no_relation1("engine_blocks_property_transit", "engine", name, "property");
  }
  if (!engine_output.properties.empty()) {
    std::set<std::pair<std::string, std::string>> entries;
    for (Property prop : engine_output.properties) {
      entries.insert({prop.klass, prop.value});
    }
    //output << create_relation_exist1("engine_imparts_property", "engine", name, "property", properties);
    output << create_triple_relation_exists("engine_imparts_property", "engine", name, entries, "property", "value");
  } else { // engine imparts no properties
    //output << create_has_no_relation1("engine_imparts_property", "engine", name, "property");
    output << create_has_no_triple_relation("engine_imparts_property", "engine", name, "property", "value");
  }
  if (!resources_consumed.empty()) {
    std::set<std::string> resources;
    for (Resource resource : resources_consumed) {
      resources.insert(resource.name);
    }
    output << create_relation_exist1("engine_uses_resource", "engine", name, "resource", resources);
  } else { // engine uses no resources
    output << create_has_no_relation1("engine_uses_resource", "engine", name, "resource");
  }

  return output.str();
}

} // namespace 'coresense::understanding::model' ends
