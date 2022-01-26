#include "Solution.hpp"

Solution::Solution(
    VehiclePerRegionSolution whichvehicles, 
    OrderSolution partial_, 
    VRPSolution result_
){
    this->whichvehicles = whichvehicles;
    this->partial = partial_;
    this->result = result_;
}

Solution::Solution(OrderSolution partial_, VRPSolution result_){
    this->partial = partial_;
    this->result = result_;
}

Solution::Solution(OrderSolution partail){
    this->partial = partail;
}

Solution::Solution(VehiclePerRegionSolution whichvehicles){
    this->whichvehicles = whichvehicles;
}