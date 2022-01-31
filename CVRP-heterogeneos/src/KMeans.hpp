#ifndef KMEANS_H_INCLUDED
#define KMEANS_H_INCLUDED

#pragma once

#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>

#include "Cluster.hpp"
#include "classes/class/Packet.hpp"

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
    double silhouette = 0.0;

    KMeans();
    KMeans(int K, int total_points, int total_values, int max_iterations);
    std::vector<Cluster> getClusters();
    Cluster getCluster(int index);
    int getK();
    
	void run(std::vector<Point> & points);
    double fractionPointClustering();
    double avarageSilhouetteValue();
    double silhouetteValue(Point point);
    Cluster getClusterNearest(Point point);
    double distanceEuclidian2D(
        std::vector<double> origin, 
        std::vector<double> destination
    );
    double computeAvarageDistance(Point point, Cluster & cluster);
    void printerClusters();

};

#endif // KMEANS_H_INCLUDED