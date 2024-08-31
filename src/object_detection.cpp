#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace cv;
using namespace dnn;
using namespace std;

#include "logger.h"
#include "object_detection.h"

int VEHICLE_CLASS[] = {CAR_CLASS, MOTORCYCLE_CLASS, BUS_CLASS, TRUCK_CLASS};

Inference::Inference(const string &onnxModelPath, const Size &modelInputShape, const string &classesFile, const bool &runWithCuda)
{
    this->modelPath = onnxModelPath;
    this->modelShape = modelInputShape;
    this->classesPath = classesFile;
    this->cudaEnabled = runWithCuda;

    loadOnnxNetwork();
    loadClassesFromFile(); 
}

vector<Detection> Inference::runInference(Mat &input)
{
    Mat modelInput = input;
    if (letterBoxForSquare && modelShape.width == modelShape.height)
        modelInput = formatToSquare(modelInput);

    Mat blob;
    dnn::blobFromImage(modelInput, blob, 1.0/255.0, modelShape, Scalar(), true, false);
    net.setInput(blob);

    vector<Mat> outputs;
    net.forward(outputs, net.getUnconnectedOutLayersNames());

    int rows = outputs[0].size[1];
    int dimensions = outputs[0].size[2];

    if (dimensions > rows)
    {
        rows = outputs[0].size[2];
        dimensions = outputs[0].size[1];

        outputs[0] = outputs[0].reshape(1, dimensions);
        transpose(outputs[0], outputs[0]);
    }

    float *data = (float *)outputs[0].data;

    float x_factor = modelInput.cols / modelShape.width;
    float y_factor = modelInput.rows / modelShape.height;

    vector<int> class_ids;
    vector<float> confidences;
    vector<Rect> boxes;

    for (int i = 0; i < rows; ++i)
    {
        float *classes_scores = data+4;

        Mat scores(1, this->classes.size(), CV_32FC1, classes_scores);
        Point class_id;
        double maxClassScore;

        minMaxLoc(scores, 0, &maxClassScore, 0, &class_id);

        if (maxClassScore > modelScoreThreshold)
        {   
            // Filter out Vehicles only
            if (find(begin(VEHICLE_CLASS), end(VEHICLE_CLASS), class_id.x) != end(VEHICLE_CLASS))
            {
                class_ids.push_back(class_id.x);
                confidences.push_back(maxClassScore);

                float x = data[0];
                float y = data[1];
                float w = data[2];
                float h = data[3];

                int left = int((x - 0.5 * w) * x_factor);
                int top = int((y - 0.5 * h) * y_factor);

                int width = int(w * x_factor);
                int height = int(h * y_factor);

                boxes.push_back(Rect(left, top, width, height));
            }
        }

        data += dimensions;
    }

    vector<int> nms_result;
    dnn::NMSBoxes(boxes, confidences, modelScoreThreshold, modelNMSThreshold, nms_result);

    vector<Detection> detections{};
    for (unsigned long i = 0; i < nms_result.size(); ++i)
    {
        int idx = nms_result[i];

        Detection result;
        result.class_id = class_ids[idx];
        result.confidence = confidences[idx];

        // Generate random colour for bounding box
        // random_device rd;
        // mt19937 gen(rd());
        // uniform_int_distribution<int> dis(100, 255);
        // result.color = Scalar(dis(gen),
        //                           dis(gen),
        //                           dis(gen));

        result.className = this->classes[result.class_id];

        result.box = boxes[idx];

        detections.push_back(result);
    }

    LOG("DEBUG", "Detected " + to_string(detections.size()) + " vehicles");

    return detections;
}

void Inference::loadClassesFromFile()
{   
    LOG("INFO", "Loading label classes from file");
    ifstream inputFile(classesPath);

    if (inputFile.is_open())
    {
        string classLine;
        while (getline(inputFile, classLine))
            this->classes.push_back(classLine);

        inputFile.close();
    }
    else
    {
        throw runtime_error("Failed to load label class file");
    }
}

void Inference::loadOnnxNetwork()
{   
    LOG("INFO", "Loading model");
    net = dnn::readNetFromONNX(modelPath);
    if (cudaEnabled)
    {
        LOG("INFO", "Target Device: GPU");
        net.setPreferableBackend(dnn::DNN_BACKEND_CUDA);
        net.setPreferableTarget(dnn::DNN_TARGET_CUDA);
    }
    else
    {
        LOG("INFO", "Target Device: CPU");
        net.setPreferableBackend(dnn::DNN_BACKEND_OPENCV);
        net.setPreferableTarget(dnn::DNN_TARGET_CPU);
    }
}

Mat Inference::formatToSquare(const Mat &source)
{
    LOG("DEBUG", "Converting input image to square");
    int col = source.cols;
    int row = source.rows;
    int _max = MAX(col, row);
    Mat result = Mat::zeros(_max, _max, CV_8UC3);
    source.copyTo(result(Rect(0, 0, col, row)));
    return result;
}
