#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "..\classes\class\CVRP.hpp"
#include "..\classes\class\Packet.hpp"
#include "Instance.hpp"


class SolomonInstance final : public Instance{
public:
    std::vector<Packet> packets;
    SolomonInstance();
    CVRP readInput(std::string filename);
    void parse(std::ifstream* arquivo, CVRP* problem);
    std::vector<std::string> split(const std::string& text, char sep);
};