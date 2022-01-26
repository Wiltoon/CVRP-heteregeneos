#ifndef SOLUTION_H_INCLUDED
#define SOLUTION_H_INCLUDED

#pragma once

#include "../class/models/VehiclePerRegionSolution.hpp"
#include "../class/models/OrderSolution.hpp"
#include "../class/models/VRPSolution.hpp"
#include "Vehicle.hpp"

class Solution{
public:
    VehiclePerRegionSolution whichvehicles;
    OrderSolution partial;
    VRPSolution result;
    
    Solution(
        VehiclePerRegionSolution whichvehicles, 
        OrderSolution partial, 
        VRPSolution result
    );
    Solution(OrderSolution partial, VRPSolution result);
    Solution(OrderSolution partail);
    Solution(VehiclePerRegionSolution whichvehicles);
};

#endif // SOLUTION_H_INCLUDED