#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
#include <random>

#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>

using namespace std;
using namespace cv;

struct Detection
{
    int class_id{0};
    string className{};
    float confidence{0.0};
    Scalar color{};
    Rect box{};
    int id;
};

struct DetectionTimer
{
    int currentCount{0};
    string time;
};

class TrackedVehicle
{
    public:
        TrackedVehicle() = default;
        TrackedVehicle(int id, const Point2f &initialCentroid);

        void update(const Point2f &newCentroid);    // Update the vehicle's centroid
        Point2f getPredictedCentroid() const;       // Get the predicted centroid
        Point2f getAverageCentroid() const;         // Get the average centroid over frames
        const vector<Point2f>& getCentroids() const; // Get all centroids
        int getId() const;                          // Get the vehicle's ID
        int getAge() const;                         // Get the vehicle's age (number of frames it hasn't been detected)

    private:
        int id;
        vector<Point2f> centroids;          // Centroid history
        int age;                            // Frames since the last detection
        mutable KalmanFilter kf;            // Kalman filter for centroid prediction
};

Rect getAdjustedROI(const string& roi_str, int video_width, int video_height);

void updateTimingsAndOverlayFrame(Mat& frame, const float& fps, const vector<Detection> detections, const map<int, Point2f>& trackedVehicles);

void drawROI(Mat& frame, Rect roi);

void updateTimer(const Detection& detection, const float& fps);



#endif // UTILS_H