#!/bin/bash
path_to_cfg="https://raw.githubusercontent.com/pjreddie/darknet/master/cfg/yolov2-tiny-voc.cfg"
path_to_weights="https://pjreddie.com/media/files/yolov2-tiny-voc.weights"
wget ${path_to_weights}
cp ./yolov2-tiny-voc.weights ./Examples/camera/bin/data/yolov2-tiny-voc.weights 
cp ./yolov2-tiny-voc.weights ./Examples/single_image/bin/data
wget ${path_to_cfg} 
cp ./yolov2-tiny-voc.cfg ./Examples/single_image/bin/data/
cp ./yolov2-tiny-voc.cfg ./Examples/camera/bin/data/
rm -f ./yolov2-tiny-voc.cfg
rm -f ./yolov2-tiny-voc.weights
echo "done"