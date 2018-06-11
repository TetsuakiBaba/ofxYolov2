ofxYolov2
====

## Description
ofxYolov2 is a YOLO(You Look Only Once) addon for openframeworks.
Please refer YOLO official website for more details of YOLO. https://pjreddie.com/darknet/yolo/
If you have Nvidia GPUs on your PC, It is better to use ofxDarknet, https://github.com/mrzl/ofxDarknet.
OpenCV v.3.3.1 or more includes Yolov2 library (https://docs.opencv.org/3.4.1/da/d9d/tutorial_dnn_yolo.html) in
their own package. Therefore I design ofxYolov2 with OpenCV v.3.3.1 or more.

I created this addon for beginers or learner who are going to try object detection with Yolo on OF.
Attention:  runs only CPU. not GPU.


## Usage
See Examples

![single_image screenshot](/screenshot.png)

## Install
1. Install OpenCV by brew ( $ brew install opencv )
2. Based on OpenCV version, Please re-write addon_config.mk file. End of the file, there are some description about opencv path for your system. Please re-write the version according to your OpenCV version. Current (2018/6/11) is 3.4.1_2, If you find other version on your directory tree, /usr/local/Cellar/opencv , please do not forget fill in the correct version on the addon_config.mk file.
3. Download yolov2 or yolov2-tiny weights and cfg file to data directory for each example or your application. https://pjreddie.com/darknet/yolov2/
[Attention] Please do not include OpenCV addon in OF. 

## Licence
[MIT](https://opensource.org/licenses/MIT)

## Author
[TetsuakiBaba](https://github.com/TetsuakiBaba)
