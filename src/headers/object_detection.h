#ifndef OBJECT_DETECTION_H
#define OBJECT_DETECTION_H

#include <fstream>
#include <vector>
#include <string>
#include <random>

// OpenCV / DNN / Inference
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>

#include "utils.h"

using namespace cv;
using namespace dnn;
using namespace std;

#define CAR_CLASS           int(2)
#define MOTORCYCLE_CLASS    int(3)
#define BUS_CLASS           int(5)
#define TRUCK_CLASS         int(7)

class Inference
{
    private:
        string modelPath{};
        string classesPath{};
        bool cudaEnabled{};

        // vector<string> classes{"person", "bicycle", "car", "motorcycle", "airplane", "bus", "train", "truck", "boat", "traffic light", "fire hydrant", "stop sign", "parking meter", "bench", "bird", "cat", "dog", "horse", "sheep", "cow", "elephant", "bear", "zebra", "giraffe", "backpack", "umbrella", "handbag", "tie", "suitcase", "frisbee", "skis", "snowboard", "sports ball", "kite", "baseball bat", "baseball glove", "skateboard", "surfboard", "tennis racket", "bottle", "wine glass", "cup", "fork", "knife", "spoon", "bowl", "banana", "apple", "sandwich", "orange", "broccoli", "carrot", "hot dog", "pizza", "donut", "cake", "chair", "couch", "potted plant", "bed", "dining table", "toilet", "tv", "laptop", "mouse", "remote", "keyboard", "cell phone", "microwave", "oven", "toaster", "sink", "refrigerator", "book", "clock", "vase", "scissors", "teddy bear", "hair drier", "toothbrush"};
        
        vector<string> classes;
        
        Size2f modelShape{};

        float modelConfidenceThreshold {0.50};
        float modelScoreThreshold      {0.45};
        float modelNMSThreshold        {0.50};

        bool letterBoxForSquare = true;

        dnn::Net net;

        void loadClassesFromFile();
        void loadOnnxNetwork();
        Mat formatToSquare(const Mat &source);
    
    public:
        Inference(const string &onnxModelPath, const Size &modelInputShape = {640, 640}, const string &classesFile = "", const bool &runWithCuda = true);
        vector<Detection> runInference(Mat &input);
};

#endif // OBJECT_DETECTION_H