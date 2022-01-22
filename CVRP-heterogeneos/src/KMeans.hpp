#ifndef KMEANS_H_INCLUDED
#define KMEANS_H_INCLUDED

#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>

#include "Cluster.hpp"

class KMeans{
private:
    int K;
    int total_values;
    int total_points;
    int max_iterations;
    std::vector<Cluster> clusters;
    // return ID of nearest center (uses euclidean distance)
	int getIDNearestCenter(Point point);
public:
    KMeans(int K, int total_points, int total_values, int max_iterations);
	void run(std::vector<Point> & points);
};

#endif // KMEANS_H_INCLUDED