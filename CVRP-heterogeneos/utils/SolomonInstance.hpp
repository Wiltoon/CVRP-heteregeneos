#ifndef SOLOMONINSTANCE_H_INCLUDED
#define SOLOMONINSTANCE_H_INCLUDED

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "..\classes\class\CVRP.hpp"
#include "..\classes\class\Packet.hpp"
#include "Instance.hpp"


class SolomonInstance : Instance{
private:
    std::vector<Packet> packets;

public:
    SolomonInstance();
    CVRP readInput(std::string filename, double alpha);
    void parse(std::ifstream& arquivo, CVRP& problem);
    void parseVehicles(CVRP& problem);
    std::vector<std::string> split(const std::string& text, char sep);
    std::vector<Packet> getPackets();
    void setPackets(std::vector<Packet> packets);
};

#endif // SOLOMONINSTANCE_H_INCLUDED