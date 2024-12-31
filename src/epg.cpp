#include "epg.hpp"

#include "program.hpp"

#include <iostream>

Epg::Epg(pugi::xml_document &doc) {
    for (pugi::xml_node &child : doc.first_child().children("channel")) {
        std::shared_ptr<Channel> channel(std::make_shared<Channel>(child));

        /* No "LD" channels */
        if (channel->name.find("LD") == std::string::npos) {
            channel_vec.push_back(channel);
            channel_map[channel->id] = channel;
        }
    }

    for (pugi::xml_node &child : doc.first_child().children("programme")) {
        Program program(child);

        if (channel_map.contains(program.channel_id)) {
            std::vector<Program> &program_vec = channel_map[program.channel_id]->programs;


            if (program_vec.size() == 0) {
                program.int_start = 0;
            } else {
                program.int_start = program_vec[program_vec.size() - 1].int_start +
                   program_vec[program_vec.size() - 1].duration;
            }

            program_vec.push_back(program);
        }
    }
}
