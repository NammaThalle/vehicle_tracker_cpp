#include <string>
#include <chrono>
#include <sstream>
#include <csignal>
#include <opencv2/opencv.hpp>

#include "utils.h"
#include "logger.h"
#include "video_processing.h"
#include "object_detection.h"
#include "vehicle_tracking.h"

using namespace cv;

#define PRINT_FREQUENCY int(50)

volatile sig_atomic_t keepRunning = 1;

void signalHandler(int signum) {
    LOG("INFO", "Ctrl + C Hit");
    LOG("INFO", "Exitting Application");
    keepRunning = 0;
}

bool processVideo(const string& inputPath, const string& outputPath, const string& modelPath, const string& classesFile, const string& roi_str)
{
    bool status = false;
    signal(SIGINT, signalHandler);

    try 
    {
        LOG("INFO", "Attempting to open input video file: " + inputPath);
        VideoCapture input(inputPath);

        if (!input.isOpened()) 
        {
            throw runtime_error("Error opening video file: " + inputPath);
        }

        LOG("INFO", "Input video file opened successfully: ");

        // Get Height x Width of the Video
        int width = int(input.get(CAP_PROP_FRAME_WIDTH));
        int height = int(input.get(CAP_PROP_FRAME_HEIGHT));

        LOG("INFO", "===================================================");

        ostringstream message;
        message << "Video Dimensions (H x W) = " << height << " x " << width;
        LOG("INFO", message.str());

        // Get FPS of the Video
        float fps = input.get(CAP_PROP_FPS);
        message.str("");
        message.clear();
        message << "Video FPS = " << fps;
        LOG("INFO", message.str());

        // Get total time of the video
        int totalFrames = input.get(CAP_PROP_FRAME_COUNT);
        float totalTimeInSeconds = totalFrames / fps;
        message.str("");
        message.clear();
        message << "Total video Length = " << totalTimeInSeconds << " s";
        LOG("INFO", message.str());

        LOG("INFO", "===================================================");

        // Create Video Writer
        LOG("INFO", "Attempting to create output video file: " + outputPath);
        VideoWriter output(outputPath, VideoWriter::fourcc('m', 'p', '4', 'v'), fps, Size(width, height));
        if (!output.isOpened())
        {
            throw runtime_error("Error opening output video file: " + outputPath);
        }
        
        LOG("INFO", "Output video file created successfully: ");
        // Frame to read the video
        Mat frame;

        bool runWithCuda = false;

        Inference yolov8(modelPath, Size(640, 640), classesFile, runWithCuda);
        VehicleTracker tracker;
        
        Rect roi;
        try 
        {
            LOG("INFO", "Attempting to assign user given ROI");
            roi = getAdjustedROI(roi_str, width, height);
        } 
        catch (const runtime_error& e) 
        {
            LOG("ERROR", string(e.what()));
            LOG("INFO", "Assigning default ROI: (H x W)");

            ostringstream roi_sstr;
            roi_sstr << "0,0," << width << "," << height;
            roi = getAdjustedROI(roi_sstr.str(), width, height);
        }

        LOG("INFO", "ROI assignment successful");
        LOG("INFO", "===================================================");
        LOG("INFO", "Starting ROI tracking");

        int frameCount = 0;
        double totalTime = 0.0; 

        auto start_time = std::chrono::high_resolution_clock::now();
        while(keepRunning && input.read(frame))
        {
            auto frame_start_time = std::chrono::high_resolution_clock::now();

            LOG("DEBUG", "Processing frame: " + to_string(frameCount));
            
            // 1. Crop ROI
            drawROI(frame, roi);
            Mat roi_frame = frame(roi);

            // 2. Vehicle Detection
            LOG("DEBUG", "Frame: " + to_string(frameCount) + ": Vehicle detection");
            vector<Detection> detections = yolov8.runInference(roi_frame);
            
            // 3. Vehicle Tracking
            LOG("DEBUG", "Frame: " + to_string(frameCount) + ": Vehicle tracking");
            tracker.update(detections);
            const map<int, Point2f>& trackedVehicles = tracker.getTrackedVehicles();

            // 4. Update Timer and Video Overlay
            LOG("DEBUG", "Frame: " + to_string(frameCount) + ": Video overlay");
            updateTimingsAndOverlayFrame(roi_frame, fps, detections, trackedVehicles);

            // 5. Write Video
            LOG("DEBUG", "Frame: " + to_string(frameCount) + ": Writing frames");
            output.write(frame);

            auto frame_end_time = chrono::high_resolution_clock::now();
            chrono::duration<double> frame_time = frame_end_time - frame_start_time;
            totalTime += frame_time.count();
            frameCount++;

            double processingFPS = frameCount / totalTime;

            if (frameCount % PRINT_FREQUENCY == 0)
            {
                LOG("INFO", "Processed Frame: " + to_string(frameCount) + " / " + to_string(totalFrames));
                LOG("INFO", "Average Processing FPS: " + to_string(processingFPS).substr(0, 4));
            }
        }

        LOG("INFO", "Closing input video");
        input.release();

        LOG("INFO", "Closing output video");
        output.release();

        status = true;
    } 
    catch (const exception& e) 
    {
        LOG("ERROR", "Exception caught during video processing: " + string(e.what()));
        LOG("INFO", "Exitting Application");
        return false;
    }

    return status;
}