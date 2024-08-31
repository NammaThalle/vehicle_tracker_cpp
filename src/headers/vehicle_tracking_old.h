#ifndef VEHICLE_TRACKING_H
#define VEHICLE_TRACKING_H

#include <opencv2/opencv.hpp>
#include <map>
#include <string>

using namespace std;
using namespace cv;

struct KalmanFilterParams {
    Mat stateTransitionMatrix;
    Mat measurementMatrix;
    Mat processNoiseCov;
    Mat measurementNoiseCov;
};

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
    int maxMissedFrames; // How many frames a vehicle can be missed before it's truly gone
    map<int, int> missedFrames;
    map<int, KalmanFilter> kalmanFilters;
    KalmanFilterParams kalmanFilterParams;

    // Helper function to find the closest existing centroid
    int findClosestCentroid(const Point2f& centroid) const;

    // Helper function to find a recently disappeared vehicle to re-associate
    int findRecentlyDisappearedVehicle(const Point2f& centroid) const; 
};

#endif // VEHICLE_TRACKING_H