#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "..\classes\class\CVRP.hpp"


class Instance{
    virtual CVRP readInput(std::string filename);
    virtual void parse(std::ifstream* arquivo, CVRP* problem);
};