#include "KMeans.hpp"

KMeans::KMeans(){
    
}

KMeans::KMeans(int K, int total_points, int total_values, int max_iterations) {
    this->K = K;
    this->total_points = total_points;
    this->total_values = total_values;
    this->max_iterations = max_iterations;
}

int KMeans::getIDNearestCenter(Point point) {
    double sum = 0.0, min_dist;
    int id_cluster_center = 0;

    for(int i = 0; i < total_values; i++)
    {
        sum += pow(clusters[0].getCentralValue(i) -
                    point.getValue(i), 2.0);
    }

    min_dist = sqrt(sum);

    for(int i = 1; i < K; i++)
    {
        double dist;
        sum = 0.0;

        for(int j = 0; j < total_values; j++)
        {
            sum += pow(clusters[i].getCentralValue(j) -
                        point.getValue(j), 2.0);
        }

        dist = sqrt(sum);

        if(dist < min_dist)
        {
            min_dist = dist;
            id_cluster_center = i;
        }
    }

    return id_cluster_center;
}

std::vector<Cluster> KMeans::getClusters(){
    return this->clusters;
}

Cluster KMeans::getCluster(int index){
    return this->clusters[index];
}

int KMeans::getK(){
    return this->K;
}

void KMeans::run(std::vector<Point> & points) {
    if(K > total_points) {
        return;
    }
    std::vector<int> prohibited_indexes;
    // choose K distinct values for the centers of the clusters
    for(int i = 0; i < K; i++) {
        while(true) {
            int index_point = rand() % total_points;
            if(find(prohibited_indexes.begin(), prohibited_indexes.end(),
                    index_point) == prohibited_indexes.end()) {
                prohibited_indexes.push_back(index_point);
                points[index_point].setCluster(i);
                Cluster cluster(i, points[index_point]);
                clusters.push_back(cluster);
                break;
            }
        }
    }
    int iter = 1;

    while(true) {
        bool done = true;
        // associates each point to the nearest center
        for(int i = 0; i < total_points; i++) {
            int id_old_cluster = points[i].getCluster();
            int id_nearest_center = getIDNearestCenter(points[i]);
            if(id_old_cluster != id_nearest_center) {
                if(id_old_cluster != -1) {
                    clusters[id_old_cluster].removePoint(points[i].getID());
                }
                points[i].setCluster(id_nearest_center);
                clusters[id_nearest_center].addPoint(points[i]);
                done = false;
            }
        }

        // recalculating the center of each cluster
        for(int i = 0; i < K; i++) {
            for(int j = 0; j < total_values; j++) {
                int total_points_cluster = clusters[i].getTotalPoints();
                double sum = 0.0;
                if(total_points_cluster > 0) {
                    for(int p = 0; p < total_points_cluster; p++) {
                        sum += clusters[i].getPoint(p).getValue(j);
                    }
                    clusters[i].setCentralValue(j, sum / total_points_cluster);
                }
            }
        }
        if(done == true || iter >= max_iterations) {
            std::cout << "Break in iteration " << iter << "\n\n";
            break;
        }
        iter++;
    }
}

double KMeans::fractionPointClustering(){
    double si = 0.0;
    double overAverages = 0.0;
    double total = 0.0;
    for(Cluster c : getClusters()) {
        for(Point p : c.getPoints()){
            si = silhouetteValue(p);
            if(si > this->silhouette){
                overAverages ++;
            }
            total++;
        }
    }
    return overAverages/total;
}

double KMeans::avarageSilhouetteValue(){
    double si = 0.0;
    double total = 0.0;
    for(Cluster c : getClusters()) {
        for(Point p : c.getPoints()){
            si += silhouetteValue(p);
            total++;
        }
    }
    if(total != 0){
        this->silhouette = si/total;
        return si/total;
    } else {
        return si;
    }
}

double KMeans::silhouetteValue(Point point){
    Cluster current = this->getCluster(point.getCluster());
    Cluster nearest = getClusterNearest(point);
    double a = computeAvarageDistance(point, current);
    double b = computeAvarageDistance(point, nearest);
    double den = (b > a) ? b : a;
    return (b-a)/den;
}

Cluster KMeans::getClusterNearest(Point point){
    double distanceMax = 0.0;
    Cluster clusterNearest = Cluster();
    for(Cluster c : clusters){
        double distance = distanceEuclidian2D(
            point.getValues(),
            c.getCentralValues()
        );
        if(distance > distanceMax){
            distanceMax = distance;
            clusterNearest = c;
        }
    }
    return clusterNearest;
}

double KMeans::distanceEuclidian2D(
    std::vector<double> origin, 
    std::vector<double> destination
){
    double dx = (double)origin[0] - (double)destination[0];
    double dy = (double)origin[1] - (double)destination[1];
    return sqrt(dx * dx + dy * dy);
}

double KMeans::computeAvarageDistance(Point point, Cluster & cluster){
    double totalDistance = 0.0;
    int total = 0;
    for (Point p : cluster.getPoints()){
        if(p.getID() != point.getID()){
            total++;
            totalDistance += distanceEuclidian2D(
                p.getValues(),
                point.getValues()
            );
        }
    }
    if(total == 0){
        return totalDistance;
    } else {
        return totalDistance/total;
    }
}

void KMeans::printerClusters(){
    // shows elements of clusters
    for(int i = 0; i < K; i++) {
        int total_points_cluster =  clusters[i].getTotalPoints();
        std::cout << "Cluster " << clusters[i].getID() + 1 << std::endl;
        for(int j = 0; j < total_points_cluster; j++) {
            std::cout << "Point " << clusters[i].getPoint(j).getID() + 1 << ": ";
            for(int p = 0; p < total_values; p++) {
                std::cout << clusters[i].getPoint(j).getValue(p) << " ";
            }
            std::string point_name = clusters[i].getPoint(j).name;
            if(point_name != ""){
                std::cout << "- " << point_name;
            }
            std::cout << std::endl;
        }
        std::cout << "Cluster values: ";
        for(int j = 0; j < total_values; j++) {
            std::cout << clusters[i].getCentralValue(j) << " ";
        }
        std::cout << "\n\n";
    }
}
