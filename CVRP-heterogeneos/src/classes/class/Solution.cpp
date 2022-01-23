#include "Solution.hpp"

Solution::Solution(OrderSolution partial_, VRPSolution result_){
    this->partial = partial_;
    this->result = result_;
}

Solution::Solution(OrderSolution partail){
    this->partial = partail;
}