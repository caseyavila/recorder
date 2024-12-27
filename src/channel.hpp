#pragma once

#include <set>
#include <string>
#include <vector>
#include <pugixml.hpp>

#include "program.hpp"

class Channel {
  private:
    static int gen_num(pugi::xml_node &node, bool integer) {
        std::string num(node.first_child().next_sibling().child_value());
        size_t dot_pos = num.find('.');

        if (integer) {
            return std::stoi(num.substr(0, dot_pos));
        } else {
            return std::stoi(num.substr(dot_pos + 1));
        }
    }

    static std::string gen_name(pugi::xml_node &node) {
        return node.first_child().next_sibling().next_sibling().child_value();
    }

  public:
    const std::string id;
    const int major_num;
    const int minor_num;
    const std::string name;
    std::vector<Program> programs;

    Channel(pugi::xml_node &node);

    std::string num_string() const;
};
