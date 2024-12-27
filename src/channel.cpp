#include "channel.hpp"

Channel::Channel(pugi::xml_node &node) :
    id(node.attribute("id").value()),
    major_num(gen_num(node, true)),
    minor_num(gen_num(node, false)),
    name(gen_name(node)) { }

std::string Channel::num_string() const {
    return std::to_string(major_num) + "." + std::to_string(minor_num);
}
