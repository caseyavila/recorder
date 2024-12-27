#pragma once

#include <pugixml.hpp>
#include <ctime>

struct Program {
    const std::string channel_id;
    const std::string title;
    const std::string subtitle;
    const std::string description;
    std::tm start_time;
    std::tm end_time;

    Program(pugi::xml_node &node);
};
