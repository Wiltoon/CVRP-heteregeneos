#include "CVRP.hpp"
#include <cmath>
#include "../../src/CVRPSolution.hpp"
#include "Packet.hpp"

CVRP::CVRP(){
    
};

void CVRP::calculate_matrix_distance(){
    for(int i = 0; i < packets.size(); i++){
        for(int j = 0; j < packets.size(); j++){
            matrix_distance[i][j] = distance_euclidian(
                packets[i],packets[j]
            );
        }
    }
};
double CVRP::distance_euclidian(Packet origin, Packet destiny){
    double dx = (double)destiny.loc_x - (double)origin.loc_x;
    double dy = (double)destiny.loc_y - (double)origin.loc_y;
    return sqrt(dx * dx + dy * dy);
};
void CVRP::calculate_matrix_price(){
    //matrix de preço para cada veiculo
};

CVRPSolution CVRP::solve(){
    //solving
    CVRPSolution problem;
    return problem;
};