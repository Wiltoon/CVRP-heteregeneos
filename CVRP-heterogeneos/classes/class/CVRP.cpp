#include "CVRP.hpp"
#include <cmath>
#include "../../src/CVRPSolution.hpp"
#include "Packet.hpp"
#include <algorithm>

CVRP::CVRP(int N, int K){
    matrix_distance = (double**) malloc(N * sizeof(double*));
    for (int i = 0; i < N; i++) {
        matrix_distance[i] = (double*)malloc(N * sizeof(double));
    }
    matrix_price = (double**) malloc(N * sizeof(double));
    for (int i = 0; i < N; i++){
        matrix_price[i] = (double*)malloc(K * sizeof(double));
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
    //O preço a se pagar para entregar o packet pelo veiculo K
    for(int i = 0; i < this->packets.size(); i++){
        for(int k = 0; k < this->vehicles.size(); k++){
            matrix_price[i][k] = price_packet_per_vehicle(
                packets[i], vehicles[k]
            );
        }
    }
}

CVRPSolution CVRP::solve(){
    //solving
    CVRPSolution problem;
    return problem;
}

double CVRP::price_packet_per_vehicle(Packet& packet, Vehicle& vehicle){
    // distancia a percorrer do deposito ate ele, e os K mais proximos
    // ordenar os pacotes mais proximos do packet
    std::vector<Packet> sorted = sortPacketsAroundPacket(packet);

}

Packet CVRP::decrescentePackets(Packet p1, Packet p2){
    return p1.price > p2.price ? p2 : p1;
}

std::vector<Packet> CVRP::sortPacketsAroundPacket(Packet pac){
    // ordenar dos pacotes mais proximos do pacote "pac"
    std::sort(packets.begin(), packets.end(), decrescentePackets);
}