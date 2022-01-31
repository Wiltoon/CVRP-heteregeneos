#include "Point.hpp"

Point::Point(){
    this->id_cluster = -1;
    this->total_values = -1;
    this->name = "dot";
    this->id_point = -1;
}

Point::Point(int id_point, std::vector<double>& values, std::string name){
    this->id_point = id_point;
    total_values = values.size();
    for(int i = 0; i < total_values; i++)
        this->values.push_back(values[i]);

    this->name = name;
    id_cluster = -1;
}

int Point::getID(){
    return id_point;
}

void Point::setCluster(int id_cluster){
    this->id_cluster = id_cluster;
}

int Point::getCluster(){
    return id_cluster;
}

double Point::getValue(int index){
    return values[index];
}

std::vector<double> Point::getValues() {
    return this->values;
}

int Point::getTotalValues(){
    return total_values;
}

void Point::addValue(double value){
    values.push_back(value);
}
