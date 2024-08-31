#include <iostream>

#include "logger.h"
#include "video_processing.h"

using namespace std;

int main(int argc, char* argv[])
{   
    int status = 1;

    LOG("INFO", "=======================================================");
    LOG("INFO", "==========||===============================||==========");
    LOG("INFO", "==========||                               ||==========");
    LOG("INFO", "==========||        VEHICLE TRACKING       ||==========");
    LOG("INFO", "==========||     Using Computer Vision     ||==========");
    LOG("INFO", "==========||                               ||==========");
    LOG("INFO", "==========||===============================||==========");
    LOG("INFO", "=======================================================");

    if (argc != 6) {
        LOG("ERROR", "Usage: vehicle_tracker <modelPath> <inputPath> <outputPath> <classesFile> <roi_str>");
        return status;
    }

    // Parse command-line arguments
    string modelPath = argv[1];
    string inputPath = argv[2];
    string outputPath = argv[3];
    string classesFile = argv[4];
    string roi_str = argv[5];

    // Output the received arguments for debugging
    LOG("INFO", "Model Path: " + modelPath);
    LOG("INFO", "Input Path: " + inputPath);
    LOG("INFO", "Output Path: " + outputPath);
    LOG("INFO", "Classes File: " + classesFile);
    LOG("INFO", "ROI String: " + roi_str);
    
    // Check if the video was processed
    if (processVideo(inputPath, outputPath, modelPath, classesFile, roi_str)) 
    {
        LOG("INFO", "Video processing completed successfully!");
        LOG("INFO", "Output stored in: " + outputPath);
        status = 0;
    } 
    else 
    {
        LOG("ERROR", "Video processing failed.");
        LOG("INFO", "Check logs in logs/vehicle_tracking.log");
        return status;
    }

    LOG("INFO", "=======================================================");
    LOG("INFO", "======================= End Of ========================");
    LOG("INFO", "================== VEHICLE TRACKING ===================");
    LOG("INFO", "=======================================================");

    return status;
}