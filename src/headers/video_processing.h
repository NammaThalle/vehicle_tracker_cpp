#ifndef VIDEO_PROCESSING_H
#define VIDEO_PROCESSING_H

using namespace std;

#include <string>

bool processVideo(const string& inputPath, const string& outputPath, const string& modelPath, const string& classesFile, const string& ROI);

#endif // VIDEO_PROCESSING_H