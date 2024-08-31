#include <iomanip>

#include "utils.h"
#include "logger.h"

map<int, DetectionTimer> detectionTimer;

Rect getAdjustedROI(const string& roi_str, int video_width, int video_height) 
{
    vector<int> roi_coords;
    stringstream ss(roi_str);
    int coord;
    char comma;

    while (ss >> coord) {
        roi_coords.push_back(coord);
        ss >> comma; 
    }

    if (roi_coords.size() != 4) {
        throw runtime_error("Invalid ROI format. Use x,y,w,h");
    }

    int x = max(0, min(roi_coords[0], video_width - roi_coords[2]));
    int y = max(0, min(roi_coords[1], video_height - roi_coords[3]));
    int w = min(roi_coords[2], video_width - x);
    int h = min(roi_coords[3], video_height - y);

    return Rect(x, y, w, h);
}

void updateTimingsAndOverlayFrame(Mat& frame, const float& fps, const vector<Detection> detections, const map<int, Point2f>& trackedVehicles)
{
    for (const auto& [id, centroid] : trackedVehicles)
    {
        // Find the detection with the matching ID
        const Detection& detection = *find_if(detections.begin(), detections.end(), 
                [id](const Detection& d) { return d.id == id; });

        updateTimer(detection, fps);

        Rect box = detection.box;
        Scalar color = Scalar(255, 255, 255);

        // Detection box
        rectangle(frame, box, color, 2);

        // Detection box text
        string classString = to_string(detection.id) + ": " + detection.className + ' ' + detectionTimer[detection.id].time;
        Size textSize = getTextSize(classString, FONT_HERSHEY_DUPLEX, 1, 2, 0);
        Rect textBox(box.x, box.y - 40, textSize.width + 10, textSize.height + 20);

        rectangle(frame, textBox, color, FILLED);
        putText(frame, classString, Point(box.x + 5, box.y - 10), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 0, 0), 2, 0);
    }
}

void drawROI(Mat& frame, Rect roi)
{

    Scalar color = Scalar(0, 255, 0);
    
    // ROI box
    rectangle(frame, roi, color, 4);

    string roiString = "ROI";
    Size textSize = getTextSize(roiString, FONT_HERSHEY_DUPLEX, 1, 2, 0);
    Rect textBox(roi.x, roi.y, textSize.width + 10, textSize.height + 20);
    
    rectangle(frame, textBox, color, FILLED);
    putText(frame, roiString, Point(roi.x + 5, roi.y + 30), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 0, 0), 2, 0);
}

void updateTimer(const Detection& detection, const float& fps)
{
    if (detectionTimer.find(detection.id) == detectionTimer.end())
    {
        detectionTimer[detection.id] = {0, "00:00.00"};
    }
    detectionTimer[detection.id].currentCount++;

    float seconds = float(detectionTimer[detection.id].currentCount / fps);

    // Calculate total seconds and milliseconds
    int total_seconds = int(floor(seconds));
    int milliseconds = int(round((seconds - total_seconds) * 100));
    
    // Calculate minutes and remaining seconds
    int minutes = total_seconds / 60;
    int secs = total_seconds % 60;

    ostringstream oss;
    oss << setw(2) << setfill('0') << minutes << ':' << setw(2) << setfill('0') << secs << '.' << setw(2) << setfill('0') << milliseconds;
    detectionTimer[detection.id].time = oss.str();
}

