#ifndef LOGGIBUD_INSTANCE_H_INCLUDED
#define LOGGIBUD_INSTANCE_H_INCLUDED

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <json/json.h>

#include "../src/classes/class/CVRP.hpp"
#include "Instance.hpp"
#include "../src/classes/class/Packet.hpp"
#include "../src/classes/class/Vehicle.hpp"

class LoggibudInstance : public Instance{
public:
    std::string nameInstance;
    std::string region;
    std::vector<Packet> packets;
    int capacity_homogeneous;
    LoggibudInstance();
    CVRP readInput(
        std::string filename, 
        std::string filevehicle, 
        int N, int K
    ) override;
    void parse(
        std::ifstream& arquivo, 
        std::string filevehicle, 
        CVRP& problem
    ) override;
    std::vector<std::string> split(const std::string& text, char sep);
};

#endif // LOGGIBUD_INSTANCE_H_INCLUDED