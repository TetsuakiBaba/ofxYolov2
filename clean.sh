#!/bin/bash
rm  ./example_camera/bin/data/yolov2-tiny.weights 
rm  ./example_single_image/bin/data/yolov2-tiny.weights 
rm  ./example_annotation/bin/data/yolov2-tiny.weights 
rm  ./example_single_image/bin/data/yolov2-tiny.cfg
rm  ./example_camera/bin/data/yolov2-tiny.cfg
rm  ./example_annotation/bin/data/yolov2-tiny.cfg
rm -fR ./example_camera/bin/camera*.app
rm -fR ./example_single_image/bin/single_image*.app
rm -fR ./example_annotation/bin/annotation*.app
echo "done"