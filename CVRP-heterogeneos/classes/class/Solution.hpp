#pragma once

#include "Vehicle.hpp"

class Solution{
public:
    Vehicle** vehicles;
    virtual Solution getOutput() = 0;
};