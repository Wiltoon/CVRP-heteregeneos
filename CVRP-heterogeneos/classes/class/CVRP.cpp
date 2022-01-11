#include "CVRP.hpp"
#include <cmath>

CVRP::CVRP(){

};

void CVRP::calculate_matrix_distance(){
    for(int i = 0; i < this.packets_order.size(); i++){
        for(int j = 0; j < this.packets_order.size(); j++){
            this.matrix_distance[i][j] = distance_euclidian(
                this.packets_order[i],this.packets_order[j]
            );
        }
    }
};
double CVRP::distance_euclidian(Packet origin, Packet destiny){
    double dx = (double)destiny.x - (double)origin.x;
    double dy = (double)destiny.y - (double)origin.y;
    return sqrt(dx * dx + dy * dy);
};
void CVRP::calculate_matrix_price(){
    //matrix de preço para cada veiculo
};

CVRPSolution CVRP::solve(){
    //solving
    CVRP problem;
    return problem;
};