#include "CVRP.hpp"

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
void CVRP::calculate_matrix_price(double alpha){
    //matrix de preço para cada veiculo
    //O preço a se pagar para entregar o packet pelo veiculo K
    for(int i = 0; i < this->packets.size(); i++){
        for(int k = 0; k < this->vehicles.size(); k++){
            matrix_price[i][k] = price_packet_per_vehicle(
                this->packets[i], this->vehicles[k], alpha
            );
        }
    }
}

CVRPSolution CVRP::solve(){
    //solving
    CVRPSolution problem;
    //criar o problema da "mochila multipla"
    //resolver o problema da "mochila multipla"
    //passar dados para o VRP
    //por fim resolver o VRP de cada veiculo 
    return problem;
}

double CVRP::price_packet_per_vehicle(
    Packet& packet, Vehicle& vehicle, double alpha
){
    // distancia a percorrer do deposito ate ele, e os K mais proximos
    // ordenar os pacotes mais proximos do packet
    std::vector<NeighborPacket> sorted = sortPacketsAroundPacket(packet);
    double CAP_PROBABILITY = alpha * vehicle.charge_max;
    double price_dist = distance_euclidian(packets[0], packet);
    vehicle.addPacket(packet);
    int cap = vehicle.current_charge;
    for(NeighborPacket nbp : sorted){
        cap += nbp.getDestination().charge;
        if(cap < CAP_PROBABILITY){
            vehicle.addPacket(nbp.getDestination());
            price_dist += distance_euclidian(
                nbp.getOrigin(), nbp.getDestination());
        } 
    }  
    return (price_dist * vehicle.cust);
}

bool CVRP::crescentePackets(const NeighborPacket& p1, const NeighborPacket& p2){
    return (p1.distance < p2.distance);
}

std::vector<NeighborPacket> CVRP::sortPacketsAroundPacket(Packet pac){
    // ordenar dos pacotes mais proximos do pacote "pac"
    // armazenar em um vetor em ordem crescente as distancias
    int N = packets.size();
    std::vector<NeighborPacket> pkts_bor;
    //somente os pedidos devem aparecer aqui
    for(int i = 1; i < N; i++){
        NeighborPacket nbp = NeighborPacket(pac,packets[i]);
        pkts_bor.push_back(nbp);
    }
    std::sort(pkts_bor.begin(), pkts_bor.end(), crescentePackets);
    return pkts_bor;
}