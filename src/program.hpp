#pragma once

#include <pugixml.hpp>
#include <ctime>

class Program {
  public:
    const std::string channel_id;
    const std::string title;
    const std::string subtitle;
    const std::string description;
    const int duration;
    int int_start;
    std::tm start_time;
    std::tm end_time;

    Program(pugi::xml_node &node);
};
