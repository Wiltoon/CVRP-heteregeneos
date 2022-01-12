#include "CVRP.hpp"
#include <cmath>
#include "../../src/CVRPSolution.hpp"
#include "Packet.hpp"

CVRP::CVRP(int num_pedidos){
    matrix_distance = (double**) malloc(num_pedidos * sizeof(double*));
    for (int i = 0; i < num_pedidos; i++) {
        matrix_distance[i] = (double*)malloc(num_pedidos * sizeof(double));
    }
}

void CVRP::calculate_matrix_distance(){

    for(int i = 0; i < this->packets.size(); i++){
        for(int j = 0; j < this->packets.size(); j++){
            if (i == j) {
                matrix_distance[i][j] = 999999.0;
            }
            else {
                matrix_distance[i][j] = distance_euclidian(
                    this->packets[i], this->packets[j]
                );
            }
        }
    }
}
double CVRP::distance_euclidian(Packet origin, Packet destiny){
    double dx = (double)destiny.loc_x - (double)origin.loc_x;
    double dy = (double)destiny.loc_y - (double)origin.loc_y;
    return sqrt(dx * dx + dy * dy);
}
void CVRP::calculate_matrix_price(){
    //matrix de preço para cada veiculo
}

CVRPSolution CVRP::solve(){
    //solving
    CVRPSolution problem;
    return problem;
}