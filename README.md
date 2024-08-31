# Vehicle Tracking and ROI Timer Application

This C++ application leverages computer vision and deep learning to track vehicles in a video and implement a timer within a designated Region of Interest (ROI). The output is a processed video with bounding boxes, ROI visualization, and wait times displayed for each vehicle within the ROI.

## Features

* **Vehicle Detection:** Employs a YOLOv8 model for real-time vehicle identification within the video stream.
* **Vehicle Tracking:** Tracks vehicles across frames to maintain identity and calculate wait times.
* **ROI Timer:** Implements a timer for each vehicle present within the user-defined ROI.
* **Video Output:** Generates a processed video with bounding boxes, ROI, and wait time overlays.

## Project Structure

* `CMakeLists.txt`: CMake build configuration.
* `Dockerfile`: Instructions for building a Docker container for the application.
* `README.md`: This documentation file.
* `build.sh`: Shell script for building the application.
* `docs`: Directory for additional documentation.
* `input`: Directory containing input videos and model configuration files.
* `logs`: Directory for application logs.
* `models`: Directory for storing the YOLOv8 model file.
* `output`: Directory for storing the processed output video.
* `run.sh`: Shell script for running the application.
* `src`: Directory containing the C++ source code.

## Program Flow

1. **Initialization:**
   * **Input Handling:** Reads command-line arguments or configuration to get the input video path, output video path, model path, classes file, and ROI coordinates.
   * **Video Capture:** Opens the input video file using OpenCV's `VideoCapture`.
   * **Video Writer:** Creates an output video file using OpenCV's `VideoWriter`.
   * **Model Loading:** Loads the YOLOv8 object detection model.
   * **Vehicle Tracker Initialization:** Sets up the vehicle tracking system.
   * **ROI Definition:** Processes the ROI coordinates and establishes the ROI region.

2. **Frame Processing Loop:**
   * **Frame Reading:** Reads a frame from the input video.
   * **ROI Cropping:** Crops the ROI region from the frame.
   * **Vehicle Detection:** Performs vehicle detection on the ROI-cropped frame using the YOLOv8 model.
   * **Vehicle Tracking:** Updates the vehicle tracker with the detected vehicles.
   * **Timer and Overlay:** Calculates wait times for vehicles within the ROI and overlays bounding boxes, ROI, and wait times on the frame.
   * **Frame Writing:** Writes the processed frame to the output video.

3. **Termination:**
   * **Cleanup:** Releases video capture and writer objects.
   * **Logging:** Logs any errors or exceptions encountered during processing.

## Usage

1. **Build:**
   * Execute `build.sh` to Build the Docker image using the provided `Dockerfile` and to compile the C++ code and generate the executable.

2. **Run:**
   * Execute `run.sh` providing the required arguments, to run the application within the Docker container, mapping the necessary volumes for input and output files:
      * `input_video_path`: Path to the input video file.
      * `output_video_path`: Path to save the processed output video.
      * `model_path`: Path to the YOLOv8 model file.
      * `classes_file`: Path to the file containing class names (e.g., `coco.names`).
      * `roi_coordinates`: ROI coordinates in the format "x,y,width,height".

## Dependencies

* OpenCV
* C++17
* Docker

## Note

* Ensure you have the required dependencies installed.
* Adjust the paths in the scripts and code according to your project structure.