// #ifndef VEHICLE_TRACKING_H
// #define VEHICLE_TRACKING_H

// #include <opencv2/opencv.hpp>
// #include <map>
// #include <string>

// #include "utils.h"

// using namespace std;
// using namespace cv;

// class VehicleTracker {
// public:
//     VehicleTracker();

//     // Update tracking based on new detections
//     void update(vector<Detection>& detections);

//     // Get the current map of tracked vehicles and their centroids
//     const map<int, Point2f>& getTrackedVehicles() const;

// private:
//     int nextVehicleId;
//     map<int, Point2f> vehicleCentroids;
//     map<int, int> vehicleAge; // Keeps track of the age of each vehicle
//     double maxDistance; // Maximum distance for matching centroids
//     int maxAge; // Maximum age before a vehicle is considered lost

//     // Helper function to find the closest existing centroid
//     int findClosestCentroid(const Point2f& centroid) const;
// };

// #endif // VEHICLE_TRACKING_H

#ifndef VEHICLE_TRACKING_H
#define VEHICLE_TRACKING_H

#include <opencv2/opencv.hpp>
#include <map>
#include <string>

using namespace std;
using namespace cv;

class VehicleTracker {
public:
    VehicleTracker();

    // Update tracking based on new detections
    void update(vector<Detection>& detections);

    // Get the current map of tracked vehicles and their centroids
    const map<int, Point2f>& getTrackedVehicles() const;

private:
    int nextVehicleId;
    map<int, Point2f> vehicleCentroids;
    double maxDistance; // Maximum distance for matching centroids

    // Helper function to find the closest existing centroid
    int findClosestCentroid(const Point2f& centroid) const;
};

#endif // VEHICLE_TRACKING_H