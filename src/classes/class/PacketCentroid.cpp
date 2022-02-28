#include "PacketCentroid.hpp"

PacketCentroid::PacketCentroid() {

}

PacketCentroid::PacketCentroid(
    int id_cluster,
    std::vector<double> center_,
    std::vector<Packet> packets_local_
) {
    this->id_cluster = id_cluster;
    this->center = center_;
    this->packets_local = packets_local_;
    for (Packet pack : packets_local_) {
        this->charge_total += pack.charge;
    }
}

double PacketCentroid::getCenterX(){
    return center[0];
}

double PacketCentroid::getCenterY(){
    return center[1];
}
