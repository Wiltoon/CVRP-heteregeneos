#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

#pragma once

#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>

class Point {
private:
    int id_cluster;
	std::vector<double> values;
	int total_values;
public:
	int id_point;
	std::string id_s;
	std::string name;
	Point();
	Point(int id_point, std::vector<double>& values, std::string name = "");
	Point(std::string id_point, std::vector<double>& values, std::string name = "");
	int getID();
	void setCluster(int id_cluster);
	int getCluster();
	double getValue(int index);
	std::vector<double> getValues();
	int getTotalValues();
	void addValue(double value);
};

#endif // POINT_H_INCLUDED