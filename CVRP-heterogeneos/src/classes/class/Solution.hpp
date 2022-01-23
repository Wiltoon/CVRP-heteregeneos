#ifndef SOLUTION_H_INCLUDED
#define SOLUTION_H_INCLUDED

#pragma once

#include "../class/models/OrderSolution.hpp"
#include "../class/models/VRPSolution.hpp"
#include "Vehicle.hpp"

class Solution{
public:
    OrderSolution partial;
    VRPSolution result;
    
    Solution(OrderSolution partial, VRPSolution result);
    Solution(OrderSolution partail);
};

#endif // SOLUTION_H_INCLUDED