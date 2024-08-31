modelPath="models/yolov8m.onnx"
inputPath="input/input_video3.mp4"
outputPath="output/output_video.mp4"
classesFile="input/coco.names"
roi_str="100,100,1680,800"

rm -rf logs output

mkdir logs output

docker run --rm \
    -v $(pwd)/logs:/app/logs \
    -v $(pwd)/output:/app/output \
    vehicle_tracker $modelPath $inputPath $outputPath $classesFile $roi_str