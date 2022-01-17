#ifndef SOLUTION_H_INCLUDED
#define SOLUTION_H_INCLUDED

#pragma once

#include "Vehicle.hpp"

class Solution{
public:
    Vehicle** vehicles;
    virtual Solution getOutput() = 0;
};

#endif // SOLUTION_H_INCLUDED