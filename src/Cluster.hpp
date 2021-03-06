#ifndef CLUSTER_H_INCLUDED
#define CLUSTER_H_INCLUDED

#pragma once

#include <iostream>
#include <vector>

#include "Point.hpp"
#include "classes/class/Packet.hpp"

class Cluster{
private:
    int id_cluster;
    std::vector<double> central_values;
    std::vector<Point> points;
public:
	std::vector<Packet> allPacks;
	Cluster();
	Cluster(int id_cluster, Point point);
	void addPoint(Point point);
	void addPacket(Packet packet);
	bool removePoint(int id_point);
	double getCentralValue(int index);
	std::vector<double> getCentralValues();
	void setCentralValue(int index, double value);
	Point getPoint(int index);
	int getTotalPoints();
	int getID();

	std::vector<Packet> getPackets(std::vector<Packet> packets);
	std::vector<Point> getPoints();
};

#endif // CLUSTER_H_INCLUDED