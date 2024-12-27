#pragma once

#include <map>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <pugixml.hpp>

#include "channel.hpp"

struct Epg {
    std::vector<std::shared_ptr<Channel>> channel_vec;
    std::unordered_map<std::string, std::shared_ptr<Channel>> channel_map;

    Epg(pugi::xml_document &doc);
};
