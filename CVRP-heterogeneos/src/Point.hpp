#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>

class Point
{
private:
	int id_point;
    int id_cluster;
	std::vector<double> values;
	int total_values;
	std::string name;
public:
	Point(int id_point, std::vector<double>& values, std::string name = "");
	int getID();
	void setCluster(int id_cluster);
	int getCluster();
	double getValue(int index);
	int getTotalValues();
	void addValue(double value);
	std::string getName();
};

#endif // POINT_H_INCLUDED