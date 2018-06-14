ofxYolov2
====

## Description
ofxYolov2 is a YOLO(You Look Only Once) addon for openframeworks.
Please refer YOLO official website for more details of YOLO. https://pjreddie.com/darknet/yolo/
If you have a Nvidia GPU(s) on your PC, I just recommend to use ofxDarknet instead of this addon, https://github.com/mrzl/ofxDarknet.

OpenCV v.3.3.1 or upper includes Yolov2 library ( https://docs.opencv.org/3.4.1/da/d9d/tutorial_dnn_yolo.html ) in
their own package. Therefore I designed ofxYolov2 with OpenCV v.3.3.1 or upper version.

I created this addon for beginers or learner who are going to try object detection with Yolo on OF.
Attention:  runs only CPU. not GPU.


## Usage
See Examples

![single_image screenshot](/screenshot.png)

## Install
1. Install OpenCV by brew ( $ brew install opencv )
2. Based on OpenCV version, Please re-write addon_config.mk file. End of the file, there are some description about opencv path for your system. Please re-write the version according to your OpenCV version. Current (2018/6/15) is 3.4.1_5, If you find other version on your directory tree, /usr/local/Cellar/opencv , please do not forget fill in the correct version on the addon_config.mk file.
3. Import Example/camera or single_image by project generator and add only ofxYolov2 as an addon.  [Attention] Please do not include OpenCV addon in OF.  Be sure to add only ofxYolov2 addon by projectgenerator. 

## Yolov2 and Yolov2-tiny
 I've include yolov2-tiny.weights and cfg file in this repository. On the other hand, Yolov2 network is more accurate than yolov2-tiny, and slow. If you try yolov2 network, you can download cfg and weights file from yolo website by yourself.

## Compatibility
- only macOS ( tested on Hight Sierra and Sierra)
- of version: 0.9.8

## Licence
[MIT](https://opensource.org/licenses/MIT)

## Author
[TetsuakiBaba](https://github.com/TetsuakiBaba)

## Reference
1. How to implement OpenCV Yolo on Openframeworks: https://qiita.com/buchayaty/items/4020100f531c07418f38
2. YOLO: https://pjreddie.com/darknet/yolo/
