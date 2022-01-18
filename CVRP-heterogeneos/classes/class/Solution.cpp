#include "Solution.hpp"

Solution(OrderSolution partial_, VRPSolution result_){
    this->partial = partial_;
    this->result = result_;
}

Solution(OrderSolution partail){
    this->partial = partail;
}