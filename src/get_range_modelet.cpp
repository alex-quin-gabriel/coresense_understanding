

#include "behaviortree_ros2/plugins.hpp"

#include <coresense_modelet_interfaces/get_range_modelet.hpp>

void GetRangeModeletNode::set_coresense_parameter() {
  const std::string path = ament_index_cpp::get_package_share_directory("coresense_modelet_interfaces") + "/config/get_range_modelet.json";
  const std::ifstream input_stream(path, std::ios_base::binary);

  if (input_stream.fail()) {
    throw std::runtime_error("Failed to open get_range_modelet.json");
  }
  std::stringstream buffer;
  buffer << input_stream.rdbuf();
  if(auto node = node_.lock()) {
    node->declare_parameter("coresense_engine", buffer.str());
  }
}
  /** 
   * @brief Required callback that allows the user to set the goal message.
   *
   * @param goal The goal to be sent to the action server.
   *
   * @return false if the request should not be sent. In that case,
   * RosActionNode::onFailure(INVALID_GOAL) will be called.
   */
bool GetRangeModeletNode::setRequest(Request::SharedPtr& request) {
  std::string modelet_id;
  getInput("modelet_id", modelet_id);
  triplestar_msgs::msg::QuerySubstitution substitution;
  substitution.variable = "modelet";
  substitution.rdf_term = modelet_id;
  request->substitutions.push_back(substitution);
  // must return true if we are ready to send the request
  return true;
}

float parse_float(std::string rdf_string) {
  return stoi(rdf_string);
}

int parse_int(std::string rdf_string) {
  return stoi(rdf_string);
}
/*
uint8 ULTRASOUND=0
uint8 INFRARED=1
std_msgs/Header header
uint8 radiation_type
float32 field_of_view
float32 min_range
float32 max_range
float32 range
*/
sensor_msgs::msg::Range create_modelet_from_json(std::string json_string) {
  sensor_msgs::msg::Range range;
  std::stringstream ss;
  ss << json_string;
  nlohmann::json result = nlohmann::json::parse(ss);
  for (nlohmann::json binding : result["results"]["bindings"]) {
    std::string modelet_id = binding["modelet_id"]["value"].get<std::string>();
    range.radiation_type = parse_int(binding[modelet_id + "_radiation_type"]["value"].get<std::string>());
    range.field_of_view = parse_float(binding[modelet_id + "_field_of_view"]["value"].get<std::string>());
    range.min_range = parse_float(binding[modelet_id + "_min_range"]["value"].get<std::string>());
    range.max_range= parse_float(binding[modelet_id + "_max_range"]["value"].get<std::string>());
    range.range = parse_float(binding[modelet_id + "_range"]["value"].get<std::string>());
  }
  return range;

}
  /** 
   * @brief Required callback invoked when the result is received by the server.
   * 
   * It is up to the user to define if the action returns SUCCESS or FAILURE.
   */
BT::NodeStatus GetRangeModeletNode::onResponseReceived(const Response::SharedPtr& response) {
  //TODO turn json into range msg
  setOutput("modelet", create_modelet_from_json(response->result));
  return BT::NodeStatus::SUCCESS;
}

  /**
   * @brief Optional callback invoked when something goes wrong. 
   *
   * It must return either SUCCESS or FAILURE.
   */
BT::NodeStatus GetRangeModeletNode::onFailure(BT::ServiceNodeErrorCode error) {
    RCLCPP_ERROR(logger(), "Error: %d", error);
    return BT::NodeStatus::FAILURE;
}


// Register this node as a plugin with the BT factory
CreateRosNodePlugin(GetRangeModeletNode, "GetRangeModelet");
