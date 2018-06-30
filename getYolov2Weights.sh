#!/bin/bash
get:
path_to_cfg="https://raw.githubusercontent.com/pjreddie/darknet/master/cfg/yolov2-tiny.cfg"
path_to_weights="https://pjreddie.com/media/files/yolov2-tiny.weights"
wget ${path_to_weights}
cp ./yolov2-tiny.weights ./Examples/camera/bin/data/yolov2-tiny.weights 
cp ./yolov2-tiny.weights ./Examples/single_image/bin/data
cp ./yolov2-tiny.weights ./Examples/annotation/bin/data
wget ${path_to_cfg} 
cp ./yolov2-tiny.cfg ./Examples/single_image/bin/data/
cp ./yolov2-tiny.cfg ./Examples/camera/bin/data/
cp ./yolov2-tiny.cfg ./Examples/annotation/bin/data/
rm -f ./yolov2-tiny.cfg
rm -f ./yolov2-tiny.weights
echo "done"