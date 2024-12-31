#include "program.hpp"

#include <sstream>
#include <iomanip>
#include <iostream>

Program::Program(pugi::xml_node &node) :
    channel_id(node.attribute("channel").value()),
    title(node.child_value("title")),
    subtitle(node.child_value("sub-title")),
    description(node.child_value("desc")),
    duration(std::stoi(node.child_value("length"))) {

    std::istringstream start(node.attribute("start").value());
    start >> std::get_time(&start_time, "%Y%m%d%H%M%S");

    std::istringstream end(node.attribute("stop").value());
    end >> std::get_time(&end_time, "%Y%m%d%H%M%S");
}
