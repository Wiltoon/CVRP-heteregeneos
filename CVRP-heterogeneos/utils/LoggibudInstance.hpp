#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "..\classes\class\CVRP.hpp"
#include "Instance.hpp"

class LoggibudInstance final : public Instance{
public:
    std::vector<Packet> packets;
    LoggibudInstance();
    CVRP readInput(std::string filename, double alpha);
    void parse(std::ifstream* arquivo, CVRP* problem);
    std::vector<std::string> split(const std::string& text, char sep);
};