#ifndef LOGGIBUD_INSTANCE_H_INCLUDED
#define LOGGIBUD_INSTANCE_H_INCLUDED

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../src/classes/class/CVRP.hpp"
#include "Instance.hpp"

class LoggibudInstance : public Instance{
public:
    std::vector<Packet> packets;
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