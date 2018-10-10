#!/bin/bash
get:
path_to_cfg="https://raw.githubusercontent.com/pjreddie/darknet/master/cfg/yolov2-tiny.cfg"
path_to_weights="https://pjreddie.com/media/files/yolov2-tiny.weights"
wget ${path_to_weights}
cp ./yolov2-tiny.weights ./example_camera/bin/data/yolov2-tiny.weights 
cp ./yolov2-tiny.weights ./example_single_image/bin/data
cp ./yolov2-tiny.weights ./example_annotation/bin/data
wget ${path_to_cfg} 
cp ./yolov2-tiny.cfg ./example_single_image/bin/data/
cp ./yolov2-tiny.cfg ./example_camera/bin/data/
cp ./yolov2-tiny.cfg ./examples_annotation/bin/data/
rm -f ./yolov2-tiny.cfg
rm -f ./yolov2-tiny.weights
echo "done"