// #include "vehicle_tracking.h"

// VehicleTracker::VehicleTracker() 
//     : nextVehicleId(0), maxDistance(50.0), maxAge(5) {} // Adjust maxDistance and maxAge as needed

// void VehicleTracker::update(vector<Detection>& detections) 
// {
//     map<int, Point2f> newCentroids;
//     map<int, int> newAges;

//     // Process new detections
//     for (auto& detection : detections) {
//         Point2f centroid(detection.box.x + detection.box.width / 2.0f, 
//                          detection.box.y + detection.box.height / 2.0f);

//         int vehicleId = findClosestCentroid(centroid);

//         if (vehicleId == -1) {
//             // New vehicle detected
//             vehicleId = nextVehicleId++;
//         }

//         newCentroids[vehicleId] = centroid;
//         newAges[vehicleId] = 0; // Reset age for detected vehicles

//         detection.id = vehicleId;
//     }

//     // Update existing centroids
//     for (const auto& [id, age] : vehicleAge) {
//         if (newCentroids.find(id) == newCentroids.end()) {
//             if (age + 1 > maxAge) {
//                 // Vehicle is lost, do not copy to new centroids
//                 continue;
//             }
//             // Vehicle was not detected but is still considered
//             newCentroids[id] = vehicleCentroids.at(id);
//             newAges[id] = age + 1;
//         }
//     }

//     vehicleCentroids = newCentroids;
//     vehicleAge = newAges;
// }

// const map<int, Point2f>& VehicleTracker::getTrackedVehicles() const {
//     return vehicleCentroids;
// }

// int VehicleTracker::findClosestCentroid(const Point2f& centroid) const {
//     int closestId = -1;
//     double minDistance = maxDistance;

//     for (const auto& [id, existingCentroid] : vehicleCentroids) {
//         double distance = norm(centroid - existingCentroid);
//         if (distance < minDistance) {
//             closestId = id;
//             minDistance = distance;
//         }
//     }

//     return closestId;
// }


#include "object_detection.h"
#include "vehicle_tracking.h"

VehicleTracker::VehicleTracker() : nextVehicleId(0), maxDistance(50.0) {} // Adjust maxDistance as needed

void VehicleTracker::update(vector<Detection>& detections) 
{
    map<int, Point2f> newCentroids;

    for (auto& detection : detections) {
        // Access bounding box information using 'box'
        Point2f centroid(detection.box.x + detection.box.width / 2.0f, 
                             detection.box.y + detection.box.height / 2.0f);

        int vehicleId = findClosestCentroid(centroid);

        if (vehicleId == -1) {
            // New vehicle detected
            vehicleId = nextVehicleId++;
        }

        newCentroids[vehicleId] = centroid;

        detection.id = vehicleId;
    }

    vehicleCentroids = newCentroids;
}

const map<int, Point2f>& VehicleTracker::getTrackedVehicles() const {
    return vehicleCentroids;
}

int VehicleTracker::findClosestCentroid(const Point2f& centroid) const {
    int closestId = -1;
    double minDistance = maxDistance;

    for (const auto& [id, existingCentroid] : vehicleCentroids) {
        double distance = norm(centroid - existingCentroid);
        if (distance < minDistance) {
            closestId = id;
            minDistance = distance;
        }
    }

    return closestId;
}