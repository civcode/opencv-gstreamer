/*
* Sender.cpp: Sender for OpenCV_GStreamer example
*
* Copyright (C) 2019 Simon D. Levy
*
* MIT License
*/

#include "pch.h"

#include <opencv2/opencv.hpp>
using namespace cv;

#include <iostream>
using namespace std;

int main()
{
    VideoCapture cap(0);
	
    if (!cap.isOpened()) {
        cerr <<"VideoCapture not opened"<<endl;
        exit(-1);
    }

	VideoWriter writer(
		"appsrc ! videoconvert ! video/x-raw,format=YUY2,width=640,height=480,framerate=60/1 ! jpegenc ! rtpjpegpay ! udpsink host=127.0.0.1 port=5000", 
        0,		// fourcc 
		60,		// fps
		Size(640, 480), 
		true);	// isColor

    if (!writer.isOpened()) {
        cerr <<"VideoWriter not opened"<<endl;
        exit(-1);
    }

    while (true) {

        Mat frame;

        cap.read(frame);

        writer.write(frame);
    }

    return 0;
}


