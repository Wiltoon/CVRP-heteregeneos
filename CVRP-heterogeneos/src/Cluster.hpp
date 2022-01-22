#ifndef CLUSTER_H_INCLUDED
#define CLUSTER_H_INCLUDED

#include <iostream>
#include <vector>

#include "Point.hpp"

class Cluster{
private:
    int id_cluster;
    std::vector<double> central_values;
    std::vector<Point> points;
public:
	Cluster(int id_cluster, Point point);
	void addPoint(Point point);
	bool removePoint(int id_point);
	double getCentralValue(int index);
	void setCentralValue(int index, double value);
	Point getPoint(int index);
	int getTotalPoints();
	int getID();
};

#endif // CLUSTER_H_INCLUDED