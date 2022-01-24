#include "PacketCentroid.hpp"

PacketCentroid::PacketCentroid(
        int id_cluster, 
        std::vector<double> center_, 
        std::vector<Packet> packets_local
    ){
        this->id_cluster = id_cluster;
        for(double coord : center_){
            this->center.push_back(coord);
        }
        for (Packet pack : packets_local){
            this->packets_local.push_back(pack);
            this->charge_total += pack.charge;
        }
}
double PacketCentroid::getCenterX(){
    return center[0];
}
double PacketCentroid::getCenterY(){
    return center[1];
}
