#ifndef SOLOMON_INSTANCE_H_INCLUDED
#define SOLOMON_INSTANCE_H_INCLUDED

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "..\classes\class\CVRP.hpp"
#include "..\classes\class\Packet.hpp"
#include "Instance.hpp"


class SolomonInstance : public Instance{
private:
    std::vector<Packet> packets;
public:
    SolomonInstance();
    CVRP readInput(std::string filename, double alpha, int N, int K) override;
    void parse(std::ifstream& arquivo, CVRP& problem) override;
    void parseVehicles(CVRP& problem);
    std::vector<std::string> split(const std::string& text, char sep);
    std::vector<Packet> getPackets();
    void setPackets(std::vector<Packet> packets);
};

#endif // SOLOMON_INSTANCE_H_INCLUDED