/*
* Sender.cpp: Sender for OpenCV_GStreamer example
*
* Copyright (C) 2019 Simon D. Levy
*
* MIT License
*/

#include "pch.h"
#include "webcam.h"

#include <opencv2/opencv.hpp>
using namespace cv;

#include <iostream>
using namespace std;

const string k_host_name = "nano.local";
const uint k_host_port = 7000;
const uint k_host_fps = 60;

const uint k_x_res = 640;
const uint k_y_res = 480;

int main()
{ 
    
    Webcam webcam("/dev/video0", k_x_res, k_y_res);
    auto frame = webcam.frame();

    auto *pt = frame.data;
    //cv::Mat img(YRES, XRES, CV_8UC3,  frame.data);
    cv::Mat img(k_y_res, k_x_res, CV_8UC3,  pt);
    
    
    auto buffer = webcam.get_buffer();
    auto *buffer_ptr = buffer.data;

    cv::Mat yuyv(k_y_res, k_x_res, CV_8UC2, buffer_ptr);
    
    cv::Mat out;

    /* 
    VideoCapture cap(0);
	
    if (!cap.isOpened()) {
        cerr << "VideoCapture not opened" << endl;
        exit(-1);
    }
    */

    //char buffer[256];
    //sprintf(buffer, "appsrc ! videoconvert ! video/x-raw,framerate=%d/1,width=640,height=480 ! videoscale ! videoconvert ! clockoverlay ! omxh265enc ! rtph265pay config-interval=1 pt=96 ! gdppay ! tcpserversink host=%s port=7000",
 
    ostringstream gst_pipeling;
    gst_pipeling << "appsrc ! videoconvert ! video/x-raw,format=YUY2,framerate=" << k_host_fps << "/1,width=640,height=480 ! videoscale ! videoconvert ! clockoverlay ! omxh265enc ! rtph265pay config-interval=1 pt=96 ! gdppay ! tcpserversink host=" << k_host_name << " port=" << k_host_port; // << "\"";
 
    cout << "\n" << gst_pipeling.str() << endl;

	VideoWriter writer(
		//"appsrc ! videoconvert ! video/x-raw,format=YUY2,width=640,height=480,framerate=60/1 ! jpegenc ! rtpjpegpay ! udpsink host=127.0.0.1 port=5000", 
        //"appsrc ! videoconvert ! video/x-raw,framerate=60/1,width=640,height=480 ! videoscale ! videoconvert ! clockoverlay ! omxh265enc ! rtph265pay config-interval=1 pt=96 ! gdppay ! tcpserversink host=192.168.0.24 port=7000",
        gst_pipeling.str(),
        0,		// fourcc 
        60,		// fps
        Size(640, 480), 
        true);	// isColor

    if (!writer.isOpened()) {
        cerr << "VideoWriter not opened" << endl;
        exit(-1);
    }

    int cnt = 0;

    while (true) {

        buffer = webcam.get_buffer();
        buffer_ptr = buffer.data;
      
        //cv::cvtColor(yuyv, out, cv::COLOR_YUV2BGR_YUYV);
        
        auto frame = webcam.frame();

        auto *pt = frame.data;
        //cv::Mat img(YRES, XRES, CV_8UC3,  frame.data);
        cv::Mat img(k_y_res, k_x_res, CV_8UC3,  pt);
        
        //cv::imshow("img", out);
        //Mat frame;

        //cap.read(frame);

        //cout << "frame " << cnt++ << endl;

        cv::cvtColor(img, out, cv::COLOR_BGR2RGB);
        //writer.write(frame);
        writer.write(out);
    }

    return 0;
}


