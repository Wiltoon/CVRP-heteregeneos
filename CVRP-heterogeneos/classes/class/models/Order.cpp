#include "Order.hpp"

void Order::createParams(){
    // Aqui tem os parametros:
    // - matrix_price, packets, vehicles, matrix custo, qi
}
void Order::createVariables(){
    // Aqui tem as variaveis de decisoes:
    // - w_k_j : essa variavel é uma entrada para o prox model
}
void Order::createFunctionObjetive(){
    // sum(pki.wkj)
}
void Order::createConstraints(){
    constraintDestiny();
    constraintCapacitedVehicle();
}
Solution Order::solve(){
    // determinar um tempo, a Solution desse modelo
    // - w_k_j : sera a matrix de destinos do prox model
}

void Order::constraintDestiny(){
    
}
void Order::constraintCapacitedVehicle(){

}